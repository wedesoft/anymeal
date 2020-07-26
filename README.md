# [AnyMeal][1] [![Build Status](https://travis-ci.org/wedesoft/anymeal.svg?branch=master)](https://travis-ci.org/wedesoft/anymeal) [![Downloads](https://img.shields.io/github/downloads/wedesoft/anymeal/total.svg)](https://github.com/wedesoft/anymeal/releases/latest)

## GNU/Linux
### Installation

Download the \*.tar.xz file of the [latest release][2].
You can then install the software under GNU/Linux as follows:

```Shell
tar xJf anymeal-*.tar.xz
cd anymeal-*/
sudo apt-get install build-essential flex googletest librecode-dev libsqlite3-dev qt5-default qttools5-dev-tools
./configure --prefix=/usr
make clean all
sudo make install
cd ..
```

You can then run anymeal:

```Shell
anymeal
```

### Deinstallation

```
cd anymeal-*/
sudo make uninstall
cd ..
```

The database can be removed as follows:

```
rm $HOME/.local/share/anymeal/anymeal.sqlite
```

## Microsoft Windows
### Installation

Download and run the \*.exe installer of the [latest release][2].
By default the installer will create a Start Menu shortcut and a desktop shortcut.

### Deinstallation

Open the Control Panel and select the applet for Programs and Features.
Select AnyMeal, then right-click and select Uninstall to remove the program.

To remove the database open the file explorer and open C:\\Users\\\<USER\>\\AppData\\Roaming.
Then select the anymeal folder, right-click, and choose Delete from the pop-up menu.

[1]: https://wedesoft.github.io/anymeal/
[2]: https://github.com/wedesoft/anymeal/releases/latest
