---
title: SafeX Vault
date: 2024-10-21 00:00:00 +0000
tags: [php, LAMP, sql]
categories: [Projects]
image: 
  path: /assets/images/safex.png
---
A no-nonsense password vault developed using the LAMP stack, with secure coding practices and robust encryption algorithms. salted hashing for authentication and AES encryption for data storage.

# SafeX
<iframe width="560" height="315" src="/assets/videos/safex.mp4" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
# Installation and Setup

### Server and MariaDB (Fedora)
Click on this [**Link**](https://tecadmin.net/install-lamp-on-fedora/) to access the documentation

### Download the zip

<div style="text-align: center; margin: 30px 0;">
  <a href="/assets/project-files/safex-vault/safex.zip" download="safex.zip" style="display: inline-block; padding: 15px 30px; background-color: #0d6efd; color: white; text-decoration: none; border-radius: 5px; font-weight: bold; box-shadow: 0 4px 6px rgba(0,0,0,0.1); transition: all 0.3s;">
    📥 Download safex.zip
  </a>
</div>

Unzip the zip file 
```
unzip safex.zip 
```
### Setup MariaDB
Change the default password values in the `DB_setup.sql` file
```
mysql -u root -p < DB_setup.sql
```

### Create a .env file in with same variable names from below file

```
DB_HOST=localhost
DB_NAME=safex

DB_USERNAME_LOGIN=safex_login
DB_PASSWORD_LOGIN="Your Password"

DB_USERNAME_SIGNUP=safex_signup
DB_PASSWORD_SIGNUP="Your Password"

DB_USERNAME_MAIN=safex_main
DB_PASSWORD_MAIN="Your Password"

KEY="A_Super_Strong_Key"
```

### Load environment variables into the PHP web app
```
composer init
composer require vlucas/phpdotenv
```

### Make the script executable and run
```
chmod +x start_safex.sh
./start_safex.sh
```

The project will be accessible at the provided link

note: works best on google chrome

