---
title: "HTTP Server with Fil-C"
date: 2023-06-17 00:00:00 +0000
tags: [C, project, fil-c, server]
categories: [Projects]
image:
  path: /assets/images/http-server.png
---

A simple HTTP server written in C that demonstrates real memory safety using Fil-C. The code contains patterns that would be easily exploitable in normal C, but Fil-C's runtime protection catches all memory safety violations.

## Installation and Execution

Step 1: Install Dependencies

First, ensure your system has the necessary tools to handle the downloads and execution environment.

```bash
# the below commands are for fedora or Red Hat based OSs, ensure you use appropriate command for your distro eg. apt for debian based systems.
sudo dnf update
sudo dnf install git wget xz python3 cmake ninja-build gcc gcc-c++ patchelf
```

Step 2: Download the Fil-C

Download and Extract: Check fil-c.org for the latest version

```bash
mkdir ~/filc_project && cd ~/filc_project
wget https://fil-c.org/download/filc-0.677-linux-x86_64.tar.xz
tar -xf filc-0.677-linux-x86_64.tar.xz
cd filc-0.677-linux-x86_64
```

Run Setup:

```bash
./setup.sh
```

Step 3: Compile with Fil-C

Instead of using standard gcc, you must use the Fil-C compiler located in the build/bin directory.

```bash
build/bin/clang -O2 -g -o server server.c
```

```
# flags 
-O2: Standard optimization.
 -g: Includes debugging information (highly recommended for Fil-C to provide better error reports).
 -o server: Names your output program.
```
Step 4: Run the Program

Now you can start your memory-safe server:

```bash
./server
```

You should see the message: Starting Server on port 8080

## Download the Script

<div style="text-align: center; margin: 30px 0;">
  <a href="/assets/project-files/http-server/server.c" download="server.c" style="display: inline-block; padding: 15px 30px; background-color: #0d6efd; color: white; text-decoration: none; border-radius: 5px; font-weight: bold; box-shadow: 0 4px 6px rgba(0,0,0,0.1); transition: all 0.3s;">
    📥 Download server.c
  </a>
</div>
