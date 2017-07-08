# A Journey through CMU Sphinx [C/Rust]

While starting my project, I noticed a huged lack of documentation regarding CMU Sphinx in C.  
Hope this can help people like me, trying to have a bit of fun with this awesome lib.


## Project & Techno

I target something simple: a simple program which will recognize what you say and react in the way you want.
My focus will mainly be on the speech recognition and make it modulable enough to easily add more action.  

The project will be split in 2 parts:
 - C: CMU Sphinx API. 2 functions, one to set the configuration and one to do the speech recognition and return the result as char[][]
 - Rust: will be the core of the app. The C code will be triggered from the Rust. Then it will parse the result and run appropriate action.

Let's go

## Install

Both document are really great regarding the installation:
Rust: https://www.rust-lang.org/en-US/install.html
CMU Sphinx: https://cmusphinx.github.io/wiki/


You can make sure CMU Sphinx is properly installed using:
```
$ pocketsphinx_continuous -inmic yes
```

After the program display 'listening', you can say something and see if Sphinx did hear you.  
If the program stay stuck at 'listening', something is wrong. Check on the documentation.

For Linux user, I suggest you to install 'pavucontrol' which let you easily manage your devices (and define the input device used by default).