#!/usr/bin/env python3
import os
import sys
import subprocess

def is_root():
    if os.getuid()!=0:
        print("You need to be root to run this program or run thisprogram with sudo")
        sys.exit(1)

def run_cmd(command):
    try:
        result=subprocess.run(command, shell=True, capture_output=True, text=True)
        return result.returncode, result.stdout, result.stderr
    except Exception as e:
        return -1, "", str(e)

def secure_ssh():
    print("Hardening SSH Config")
    ssh_config="/etc/ssh/sshd_config"

    config={
        "PermitRootLogin": "no",
        "PasswordAuthentication": "no",
        "PermitEmptyPasswords": "no"
    }
    with open(ssh_config,'r') as f:
        lines=f.readlines()
    new_lines=[]
    for line in lines:
        modified=False
        for key,value in config.items():
            if line.strip().startswith(key):
                new_lines.append(f"{key} {value}\n")
                modified=True
                break
        if not modified:
            new_lines.append(line)
 
    for key, value in config.items():
        found=any(key in line for line in new_lines)
        if not found:
            new_lines.append(f"{key} {value}\n")
    with open(ssh_config,'w') as f:
        f.writelines(new_lines)

def file_permissions():
    files_perms={
        "/etc/passwd":"644",
        "/etc/shadow":"600",
        "/etc/group":"644",
    }

    for file, perm in files_perms.items():
        os.system(f"sudo chmod {perm} {file}")

def firewall_config():  
        os.system("ufw --force enable 2>/dev/null")
        os.system("ufw default deny incoming 2>/dev/null")
        os.system("ufw default allow outgoing 2>/dev/null")
    
def check_updates():
    print("System Updates Check")
    print("Update your system regularly, be careful for distros like arch can break upon updates.")
    
    if os.path.exists("/etc/debian_version"):
        print("Run 'sudo apt update && apt upgrade'")
    elif os.path.exists("/etc/redhat-release"):
        print("Run 'sudo yum update'")
    else:
        print("Run your system's update command")
    
    response=input("Have you updated recently? (y/n): ")
    if response.lower() != 'y':
        print("Please update your system!")

def check_passwords():
    print("Password Policy Check")
    print("Ensure strong password policies")
    print("Minimum 12 characters")
    print("Password expiration (90 days)")
    print("Account lockout after failed attempts")
    
    response=input("Are password policies configured? (y/n): ")
    if response.lower() != 'y':
        print("Edit /etc/login.defs and /etc/pam.d/common-password")

def check_services_and_ports():
    print("Run command 'netstat -tuln' to review all service and their ports")
    print("if you find anything suspicious, stop those service and block the port")

def check_processes():
    print("Run 'ps aux' to review all processes")
    print("cross check suspicious ports from netstat output with the corresponding process")
    
def check_users():
    print("Run 'cat /etc/passwd' and review all user accounts")
    print("If you find any new user accounts apart from regular users and service accounts, delete the user immediately")    
    print("Remove users uing the command 'userdel <username>'")


def check_sudo():
    print("sudo config check")
    
    code, out, err = run_cmd("cat /etc/sudoers | grep -v '^#' | grep -v '^$'")
    if code==0 and out:
        print("Current sudo rules:")
        print(out)
    print("Ensure only authorized users have sudo access, typically only root user and the wheel group")
    print("Edit with: visudo")

def main():
    is_root()
    secure_ssh()
    file_permissions()
    firewall_config()
    check_updates()
    check_passwords()
    check_services_and_ports()
    check_processes()
    check_users()
    check_sudo()

if __name__ == "__main__":
    main()
