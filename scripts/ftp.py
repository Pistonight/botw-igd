"""Performs file actions on console via FTP"""
# Usage: ftpUtil.py command ip
from ftplib import FTP, all_errors
from os import makedirs
from os.path import join, isfile
import sys
# pylint: disable-next=import-error
from cu import read_ftp_config

# Directory to store the patches on switch
PATCH_DIRNAME = "botwigd"

def run(command):
    """Main"""
    ftp_config = read_ftp_config()
    console_ip = ftp_config["ip"]

    if '.' not in console_ip:
        print("Invalid IP:", console_ip)
        sys.exit(-1)

    console_port = 5000

    ftpw = FtpWrapper(console_ip, console_port)

    if command == "deploy":
        ftpw.connect()
        deploy(ftpw, ftp_config)
    elif command == "clean":
        ftpw.connect()
        clean(ftpw, ftp_config)
    elif command == "report":
        ftpw.connect()
        report(ftpw, ftp_config)
    else:
        print("Unknown command:", command)
        sys.exit(-1)

# Deploy
def deploy(ftpw, ftp_config):
    """Deploy to console"""
    for file in ftp_config["file"]:
        local_path = file["local"]
        target_path = file["target"]
        ftpw.ensure_parent(target_path)
        ftpw.send_file(local_path, target_path)

# Clean
def clean(ftpw, ftp_config):
    """Remove deployment on console"""
    for file in ftp_config["file"]:
        ftpw.delete_file(file["target"])

# Get crash report
def report(ftpw, ftp_config):
    """Download crash reports and delete them on console"""
    crash_report_dir = ftp_config["crash_reports"]
    ftpw.ensure_path(crash_report_dir)
    ftpw.retrive_directory("crash_reports", crash_report_dir)
    ftpw.delete_directory(crash_report_dir)

class FtpWrapper:
    """Wrapper for ftp"""
    def __init__(self, host, port):
        """Constructor"""
        self.ftp = FTP()
        self.host = host
        self.port = port

    def connect(self):
        """Connect"""
        print(f'Connecting to {self.host}:{self.port}... ', end='')
        self.ftp.connect(self.host, self.port)
        print('Connected!')

    def listcontent(self,sd_path):
        """Returns directories, files"""
        file_list, dirs, nondirs = [], [], []
        if not self.cwd(sd_path):
            return [], []

        self.ftp.retrlines('LIST', lambda x: file_list.append(x.split()))
        for info in file_list:
            ls_type, name = info[0], info[-1]
            if ls_type.startswith('d'):
                dirs.append(name)
            else:
                nondirs.append(name)
        return dirs, nondirs

    def ensure_parent(self, path_to_file):
        """Ensure parent of the path to file exists"""
        directories = path_to_file.split("/")[:-1]
        self.ensure_path_array(directories)

    def ensure_path(self, path):
        """Ensure directory exists"""
        directories = path.split("/")
        self.ensure_path_array(directories)

    def ensure_path_array(self,path_array):
        """Iteratively make sure path exists"""
        if len(path_array) == 0 or (len(path_array)==1 and path_array[0]=="/"):
            return

        cur_root = "/"
        index = 0
        if path_array[0] == "/":
            self.ensure_from_root("/", path_array[1])
            cur_root = join(cur_root, path_array[1])
            index = 2
        else:
            self.ensure_from_root("/", path_array[0])
            cur_root = join(cur_root, path_array[0])
            index = 1

        while index < len(path_array):
            self.ensure_from_root(cur_root, path_array[index])
            cur_root = join(cur_root, path_array[index])
            index+=1

    def ensure_from_root(self, root, path):
        """If path is not a directory under root, make one"""
        directories, _ = self.listcontent(root)
        if path not in directories:
            print(f"> MKD {join(root, path)}")
            self.ftp.mkd(f'{root}/{path}')

    def send_file(self, local_path, sd_path):
        """Send local_path to sd_path"""
        sd_path = add_slash_if_need(sd_path)
        if not isfile(local_path):
            print("Error: ", local_path, " does not exist or is not a file")
            sys.exit(-1)
        print(f"Sending {local_path}")
        print(f'> STOR {sd_path}')
        with open(local_path, 'rb') as local_file:
            self.ftp.storbinary(f'STOR {sd_path}', local_file)

    def delete_file(self, sd_path):
        """Delete sd_path"""
        sd_path = add_slash_if_need(sd_path)
        try:
            self.ftp.delete(sd_path)
            print(f'> DELE {sd_path}')
        except all_errors:
            return

    def delete_directory(self, sd_path):
        """Recursively delete directory"""
        directories, files = self.listcontent(sd_path)
        for directory in directories:
            self.delete_directory(join(sd_path, directory))
        for file in files:
            self.delete_file(join(sd_path, file))

        print(f'> RMD {sd_path}')
        self.ftp.rmd(sd_path)

    def retrive_file(self, local_path, sd_path):
        """Download sd_path to local_path"""
        sd_path = add_slash_if_need(sd_path)
        print(f"Receiving {local_path}")
        with open(local_path, "wb+") as file:
            self.ftp.retrbinary(f"RETR {sd_path}", file.write)
            print(f"> RETR {sd_path}")

    def retrive_directory(self, local_path, sd_path):
        "Recursively download directory"
        makedirs(local_path, exist_ok=True)
        directories, files = self.listcontent(sd_path)
        for directory in directories:
            self.retrive_directory(join(local_path, directory), join(sd_path, directory))
        for file in files:
            self.retrive_file(join(local_path, file), join(sd_path, file))

    def cwd(self, path):
        "Change Working Directory"
        path = add_slash_if_need(path)
        try:
            self.ftp.cwd(path)
        except all_errors:
            return False
        return True

def add_slash_if_need(path):
    """ a/b/c -> /a/b/c"""
    return path if path.startswith("/") else "/"+path

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <deploy|clean|report>")
        sys.exit(-1)
    run(sys.argv[1])
