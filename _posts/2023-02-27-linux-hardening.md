---
title: "Linux Hardening using Python"
date: 2023-02-27 00:00:00 +0000
categories: [Projects]
tags: [linux, python, hardening, project]
image:
  path: /assets/images/linux-hardening.png
---

## Overview

This script performs automated system hardening by updating the configuration and recommending some manual checks that can help you secure your system and detect any suspicious behaviour. 


## What It Does

**SSH Configuration** - Modifies /etc/ssh/sshd_config to disable root login, enforce key-based auth, and prevent empty passwords.

**File Permissions** - Sets appropriate permissions on /etc/passwd (644), /etc/shadow (600), and /etc/group (644).

**Firewall** - Enables UFW with deny-incoming/allow-outgoing policy.

**Manual Checks** - Prompts for system updates, password policies, netstat output review, process inspection, unauthorized user detection, and sudoers validation.

## Working

The script requires root privileges and uses `subprocess` to execute system commands. SSH hardening parses the existing `sshd_config`, preserves any custom settings, and enforces the security directives. File permissions are set using `chmod`, and UFW commands configure the firewall with safe defaults.

Interactive prompts guide you through manual security checks, things that need human judgment like reviewing running processes or checking for unauthorized user accounts.

## Usage Guide

**Prerequisites**: Linux OS, Python 3.x, root access, UFW installed

**To run the script:**
```bash
chmod +x linux_hardening.py
sudo ./linux_hardening.py
```

## Benefits & Use Cases

 **Consistency**: Uniform security across systems  
 **Speed**: Minutes instead of hours  
 **Compliance**: CIS Benchmark aligned  
 **Educational**: Teaches security best practices  

**Real-world applications**: Personal use, educational labs, compliance audits.

## Download the Script

<div style="text-align: center; margin: 30px 0;">
  <a href="/assets/project-files/linux-hardening/linux-hardening.py" download="linux-hardening.py" style="display: inline-block; padding: 15px 30px; background-color: #0d6efd; color: white; text-decoration: none; border-radius: 5px; font-weight: bold; box-shadow: 0 4px 6px rgba(0,0,0,0.1); transition: all 0.3s;">
    📥 Download linux-hardening.py
  </a>
</div>

## Important Notes

> **Before running**: Backup your system, test in a VM first, ensure SSH keys are configured
{: .prompt-warning }

> **After running**: Restart SSH (`sudo systemctl restart sshd`), test connectivity in a new session, review firewall rules
{: .prompt-tip }




