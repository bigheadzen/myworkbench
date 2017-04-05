# Linux Environment Setup

## Preparation

* Ubuntu 14.04 Server amd64
* OpenSSH-server installed
* Account and password provided

## Initial Config

SSH to the server.

### Setup SSH

    ssh-keygen
    vim .ssh/authorized_keys
    # Add personal pub key for passwordless SSH

### Setup HTTP Proxy (TBD)

    proxychains4

### Set root password

    sudo passwd

### Passwordless sudo

    # Add to /etc/sudoers
    %junguo ALL=(ALL) NOPASSWD: ALL


### Update sources.list config

    sudo cp /etc/apt/sources.list /etc/apt/sources.list.bk
    sudo vim /etc/apt/sources.list
    # Replace the content with the sources below.
    
    # 阿里云
    deb http://mirrors.aliyun.com/ubuntu/ trusty main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ trusty-security main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ trusty-updates main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ trusty-proposed main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ trusty-backports main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ trusty main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ trusty-security main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ trusty-updates main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ trusty-proposed main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ trusty-backports main restricted universe multiverse
    
    # 163 服务器
    deb http://mirrors.163.com/ubuntu/ vivid main restricted universe multiverse
    deb http://mirrors.163.com/ubuntu/ vivid-security main restricted universe multiverse
    deb http://mirrors.163.com/ubuntu/ vivid-updates main restricted universe multiverse
    deb http://mirrors.163.com/ubuntu/ vivid-proposed main restricted universe multiverse
    deb http://mirrors.163.com/ubuntu/ vivid-backports main restricted universe multiverse
    deb-src http://mirrors.163.com/ubuntu/ vivid main restricted universe multiverse
    deb-src http://mirrors.163.com/ubuntu/ vivid-security main restricted universe multiverse
    deb-src http://mirrors.163.com/ubuntu/ vivid-updates main restricted universe multiverse
    deb-src http://mirrors.163.com/ubuntu/ vivid-proposed main restricted universe multiverse
    deb-src http://mirrors.163.com/ubuntu/ vivid-backports main restricted universe multiverse
    
    sudo apt-get update

### Install open-vm-tools (for VMWare Guest)

    sudo apt-get install open-vm-tools
    
### Setup .bash_aliases

    curl https://raw.githubusercontent.com/bigheadzen/personalenv/master/linux/_bash_aliases > .bash_aliases
    exec $SHELL

### add-apt-repository & ppa-purge

To add a 3rd party apt repository. (take vim for example)

    sudo add-apt-repository ppa:jonathonf/vim
    sudo apt update
    sudo apt install vim

To remove a apt repository added by `add-apt-repository`, please install `ppa-purge`.  

    sudo apt install ppa-purge
    sudo ppa-purge ppa:jonathonf/vim

## Install checkinstall

From [CheckInstall](https://help.ubuntu.com/community/CheckInstall):  

>CheckInstall keeps track of all files installed by a "make install" or equivalent, creates a Slackware, RPM, or Debian package with those files, and adds it to the installed packages database, allowing for easy package removal or distribution.

    sudo apt-get install checkinstall

So instead of `sudo make install`, please run  

    sudo checkinstall # will call make install and package the result

To specify a make target, run  

    sudo checkinstall make install_package

Then the package is installed in server and an deb file generated. You can copy the deb to other machines and install.  
To remove the package just run  

    sudo dpkg -r packagename

## Install development tools

### Install git

    sudo add-apt-repository ppa:git-core/ppa
    sudo apt-get update
    sudo apt-get install git

### Install build essential

    # Install gcc, g++, make
    sudo apt-get install build-essential gdb autoconf

### Install CMake

    # Download latest stable cmake source code
    wget https://cmake.org/files/v3.7/cmake-3.7.2.tar.gz
    tar xf cmake-3.7.2.tar.gz
    cd cmake-3.7.2/
    ./configurer
    make
    sudo checkinstall
    
    exec $SHELL
    cmake --version
    
    # To remove the package just run 
    dpkg -r cmake

### Install clang

Add the [apt repository](http://apt.llvm.org/) to  `/etc/apt/sources.list`.

    deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty main
    deb-src http://apt.llvm.org/trusty/ llvm-toolchain-trusty main
    # 3.9 
    deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-3.9 main
    deb-src http://apt.llvm.org/trusty/ llvm-toolchain-trusty-3.9 main
    # 4.0 
    deb http://apt.llvm.org/trusty/ llvm-toolchain-trusty-4.0 main
    deb-src http://apt.llvm.org/trusty/ llvm-toolchain-trusty-4.0 main

Install.

    wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add -
    sudo apt-get update
    sudo apt-get install clang-4.0 lldb-4.0 lld-4.0
    
    # Add to the beginning of PATH
    vim ~/.bashrc
    export PATH=/usr/lib/llvm-4.0/bin:$PATH
    exec $SHELL
    
    # No need to update alternatives
    # sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-4.0 400
    # sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-4.0 400
    

## VIM

See [VIM Configuration in Linux](https://gist.github.com/bigheadzen/6471593c046cb74077a6263972d7d77c)

## ack

    sudo curl -o /usr/bin/ack https://beyondgrep.com/ack-2.18-single-file
    sudo chmod 755 /usr/bin/ack
    ack --version
