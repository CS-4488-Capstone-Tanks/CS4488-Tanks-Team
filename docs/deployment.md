# Deployment

_Primary Author: Koda Koziol, SPRING 2024_

These steps were tested on Windows with the windeployqt6 tool. There are similar tools for other platforms, but we haven't tried them.

## TL;DR

1. Do a release build, then open the build directory.
2. Copy the executable and the `assets` folder to the directory where you want to run the game.
3. Run `windeployqt6` on the executable, passing the path to the executable as an argument.
4. Run the executable and crush some tanks.

## Detailed Steps

### 1. Do a Release Build

This can be done through your IDE or directly through make, it depends on your specific setup. The important thing is that you build a release version of the game. If you try to run the executable as is you'll probably get a bunch of errors about missing DLLs. If you don't get any errors, it's probably because you have all the necessary DLLs in your system path, but you'll still need to bundle them with the executable to run it on other computers.

### 2. Get the Files You Need

Navigate to the release build directory where the executable is located. You will need two things: the executable `tanks.exe` and the `assets` folder. The easiest to do is to copy the `tanks.exe` executable and the `assets` folder to a new directory where you want to run the game, but you can also just delete everything else in the build directory (if Windows will let you), or leave it all there (it won't hurt anything, but it's messy).

### 3. Run `windeployqt6` to get the DLLs

`windeployqt6.exe` is a tool that's included with Qt and is used to copy all the necessary DLLs to the directory with the executable. You should be able to find it in your QT install directory, somewhere like: `C:\Qt\6.7.0\mingw_64\bin`. You'll probably see other deploy tools for other operating systems in there too, but we didn't test them. It only takes one argument, the path to the executable you want to get the DLLs for.
You can run it from the command line like this:

```bash
windeployqt6 path/to/tanks.exe
```

Now you should see a bunch of DLLs in that directory with the executable and `assets` folder.

### 4. Run the Game

Now you should be able to start the game by running the executable. If you want to share the game to another computer, you'll need to copy the entire directory with the executable, the `assets` folder, and all the DLLs.