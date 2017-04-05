# VIM Configuration in Linux

## Install from PPA repository

The easiest way is to install from PPA. **However**, the installed VIM is not configured with python and ruby support.  
So **DONOT** use this way if you need python and ruby support.

    sudo add-apt-repository ppa:jonathonf/vim
    sudo apt update
    sudo apt install vim

## Build from source

Refered from [Building Vim from source](https://github.com/Valloric/YouCompleteMe/wiki/Building-Vim-from-source)  
### Install prerequisites

    # with GUI support
    sudo apt-get install libncurses5-dev libgnome2-dev libgnomeui-dev \
        libgtk2.0-dev libatk1.0-dev libbonoboui2-dev \
        libcairo2-dev libx11-dev libxpm-dev libxt-dev python-dev \
        python3-dev ruby-dev lua5.1 lua5.1-dev libperl-dev
    
    # Only terminal support
    sudo apt-get install libncurses5-dev python-dev python3-dev ruby-dev lua5.1 lua5.1-dev libperl-dev
    
    # remove old VIM
    sudo apt-get remove vim vim-runtime gvim vim-common

### Get source

    wget -O vim-v8.0.0542.tar.gz https://github.com/vim/vim/archive/v8.0.0542.tar.gz
    
    cd vim
    ./configure --with-features=huge \
                --enable-multibyte \
                --enable-rubyinterp=yes \
                --enable-pythoninterp=yes \
                --with-python-config-dir=/usr/lib/python2.7/config-x86_64-linux-gnu \
                --enable-python3interp=yes \
                --with-python3-config-dir=/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu \
                --enable-perlinterp=yes \
                --enable-luainterp=yes \
                --enable-cscope --prefix=/usr
    make VIMRUNTIMEDIR=/usr/share/vim/vim80
    
    sudo checkinstall
    # sudo dpkg -r vim # to remove vim
    
    sudo update-alternatives --install /usr/bin/editor editor /usr/bin/vim 1
    sudo update-alternatives --set editor /usr/bin/vim
    sudo update-alternatives --install /usr/bin/vi vi /usr/bin/vim 1
    sudo update-alternatives --set vi /usr/bin/vim

## Install vimrc and vundle

    wget -O ~/.vimrc https://raw.githubusercontent.com/bigheadzen/personalenv/master/vim/_vimrc
    git clone https://github.com/VundleVim/Vundle.vim.git  ~/.vim/bundle/Vundle.vim
    mkdir -pv ~/.vim/backupfiles
    
    # open vim and run :PluginInstall

## Get C & C++ sample source code

    git clone https://github.com/mackyle/sqlite
    git clone https://github.com/google/leveldb.git
    git clone https://github.com/facebook/rocksdb.git

## ctags & cscope

    sudo apt-get install exuberant-ctags
    sudo apt-get install cscope

### ctags

    cd sqlite
    ctags -R .
    
In `.vimrc` add the ctags config below.  

    set tags=tags; 

The `;` is very important. It will load from current directory and it's ancestor directories.  
Open the code file from `sqlite` or `sqlite\src` folder, vim will load the tags in `sqlite` folder correctly.  
* `Ctrl+]` or `tag tag_name` will navigate to the tag declaration.
* `Ctrl+o` or `Ctrl+t` will navigate back.

### cscope

    cscope -Rbq

Add the config below in .vimrc if not in there.


    if has('cscope')
        set cscopetag  # use cscope DB for ctags
        set csto=0
        set cscopeverbose
    
        if has('quickfix')
            set cscopequickfix=s-,c-,d-,i-,t-,e-
        endif
    
        cnoreabbrev csa cs add
        cnoreabbrev csf cs find
        cnoreabbrev csfs cs find s
        cnoreabbrev csfc cs find c
        cnoreabbrev csfe cs find e
        cnoreabbrev csft cs find t
        cnoreabbrev csfd cs find d
        cnoreabbrev csfi cs find i
        cnoreabbrev csfg cs find g
        cnoreabbrev csk cs kill
        cnoreabbrev csr cs reset
        cnoreabbrev css cs show
        cnoreabbrev csh cs help
    
        " cnoreabbrev cs cs find s <cword>
        " cnoreabbrev cc cs find c <cword>
        " cnoreabbrev ce cs find e <cword>
        " cnoreabbrev ct cs find t <cword>
        " cnoreabbrev cd cs find d <cword>
        " cnoreabbrev ci cs find i ^<cfile>$
        " cnoreabbrev cg cs find g <cword>
        " cnoreabbrev cf cs find f <cword>
    
        " nnoremap <C-s>s :cs find s <cword><CR>
        " nnoremap <C-s>g :cs find g <cword><CR>
        " nnoremap <C-s>c :cs find c <cword><CR>
        " nnoremap <C-s>t :cs find t <cword><CR>
        " nnoremap <C-s>e :cs find e <cword><CR>
        " nnoremap <C-s>f :cs find f <cfile><CR>
        " nnoremap <C-s>i :cs find i ^<cfile>$<CR>
        " nnoremap <C-s>d :cs find d <cword><CR>
    
        set nocsverb
        if filereadable("cscope.out")    " 若当前目录下存在cscope数据库，添加该数据库到vim
            cs add cscope.out
        elseif $CSCOPE_DB != ""            " 否则只要环境变量CSCOPE_DB不为空，则添加其指定的数据库到vim
            cs add $CSCOPE_DB
        endif
        set csverb
    
        " shortcut, place the cursor on the word and press Ctrl+\ then the char immediately
        nmap <C-\>s :cs find s <C-R>=expand("<cword>")<CR><CR>
        nmap <C-\>g :cs find g <C-R>=expand("<cword>")<CR><CR>
        nmap <C-\>c :cs find c <C-R>=expand("<cword>")<CR><CR>
        nmap <C-\>t :cs find t <C-R>=expand("<cword>")<CR><CR>
        nmap <C-\>e :cs find e <C-R>=expand("<cword>")<CR><CR>
        nmap <C-\>f :cs find f <C-R>=expand("<cfile>")<CR><CR>
        nmap <C-\>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>
        nmap <C-\>d :cs find d <C-R>=expand("<cword>")<CR><CR>
    endif

### Generate tags and cscope for system libs and headers

    cd /usr/include
    ctags -R --c++-kinds=+p --fields=+iaS --extra=+q /usr/include
    sudo cscope -Rbq

Update .vimrc config.

    set tags+=/usr/include/tags
    cs add /usr/include/cscope.out /usr/include

## YouCompleteMe

### Prerequisites

**Make sure the packages below and clang are installed.**  

    sudo apt-get install build-essential cmake python-dev python3-dev

### Install for C/C++

    cd ~/.vim/bundle/YouCompleteMe
    ./install.py --clang-completer

### Setup .vimrc

这些 .vimrc 里面应该已经有了，列出来确保一致。

    " YCM 补全菜单配色
    " 菜单
    " highlight Pmenu ctermfg=2 ctermbg=3 guifg=#005f87 guibg=#EEE8D5
    " 选中项
    " highlight PmenuSel ctermfg=2 ctermbg=3 guifg=#AFD700 guibg=#106900
    
    " 补全功能在注释中同样有效
    let g:ycm_complete_in_comments=1
    " 允许 vim 加载 .ycm_extra_conf.py 文件，不再提示
    let g:ycm_confirm_extra_conf=0
    " 开启 YCM 标签补全引擎
    let g:ycm_collect_identifiers_from_tags_files=1
    
    " 引入 C++ 标准库tags, 这个前面的 ctags 配置应该已经加过了
    " set tags+=/usr/include/c++/5.4.0/tags
    
    " YCM 集成 OmniCppComplete 补全引擎，设置其快捷键
    inoremap <leader>, <C-x><C-o>
    
    " 补全内容不以分割子窗口形式出现，只显示补全列表
    set completeopt-=preview
    " 从第一个键入字符就开始罗列匹配项
    let g:ycm_min_num_of_chars_for_completion=1
    
    " 禁止缓存匹配项，每次都重新生成匹配项
    let g:ycm_cache_omnifunc=0
    " 语法关键字补全
    let g:ycm_seed_identifiers_with_syntax=1
    
    nnoremap <leader>jc :YcmCompleter GoToDeclaration<CR>
    " 只能是 #include 或已打开的文件
    " nnoremap <leader>jd :YcmCompleter GoToDefinition<CR>
    nnoremap <leader>jd :YcmCompleter GoToDefinitionElseDeclaration<CR>

### Setup .ycm_extra_conf.py

理论上我们不应该使用全局的 .ycm_extra_conf.py 配置因为每个项目的配置是不一样的。所以在每个项目的根目录上应该配置一个该文件来定义该项目所使用的构建参数, YCM 默认会自动加载。  

从 `https://raw.githubusercontent.com/bigheadzen/personalenv/master/vim/_ycm_extra_conf.py` 上下载一个模板放到自己的项目目录下。重命名成 `.ycm_extra_conf.py`，并修改成匹配当前项目的 flags 参数。
