# yaclip

Yet another clipboard tool for Windows.

## Rquirements

- [MSYS2]
- [libiconv]
- [boost]

## Installation

Execute msys2\_shell.cmd in the folder where MSYS 2 is installed with the -mingw64 command line option and execute the following commands in the MSYS2 terminal.

    git clone https://github.com/erw7/yaclip.git
    cd yaclip
    mingw32-make

## Basic usage

### Get the clipboard.

    yaclip -o

### Set the clipboard.

    echo "This is a test text." | yaclip  -i

## Use with neovim

Apply the next patch to share/nvim/runtime/autoload/provider/clipboard.vim.

```diff
--- "share/nvim/runtime/autoload/provider/clipboard.vim.orig"     2017-01-09 23:59:47.057126600 +0900
+++ "share/nvim/runtime/autoload/provider/clipboard.vim"  2017-01-10 00:00:11.798770100 +0900
@@ -69,6 +69,12 @@
     let s:copy['*'] = s:copy['+']
     let s:paste['*'] = s:paste['+']
     return 'doitclient'
+  elseif executable('yaclip')
+    let s:copy['+'] = 'yaclip -i --crlf'
+    let s:paste['+'] = 'yaclip -o --lf'
+    let s:copy['*'] = s:copy['+']
+    let s:paste['*'] = s:paste['+']
+    return 'yaclip'
   endif

   let s:err = 'clipboard: No clipboard tool available. See :help clipboard'
```

Copy yaclip.exe to the folder that included in command search path or to the folder same folder as nvim.exe.

[MSYS2]:https://msys2.github.io/
[libiconv]:https://www.gnu.org/software/libiconv/
[boost]:http://www.boost.org/
