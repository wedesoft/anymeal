# [AnyMeal][1] [![Build Status](https://travis-ci.org/wedesoft/anymeal.svg?branch=master)](https://travis-ci.org/wedesoft/anymeal) [![Downloads](https://img.shields.io/github/downloads/wedesoft/anymeal/total.svg)](https://github.com/wedesoft/anymeal/releases/latest)

## GNU/Linux (Debian based)
### Installation

Download the \*.tar.xz file of the [latest release][2].
You can then install the software under GNU/Linux as follows:

```Shell
tar xJf anymeal-*.tar.xz
cd anymeal-*/
sudo apt-get install build-essential flex googletest librecode-dev libsqlite3-dev qt5-default qttools5-dev-tools
./configure --prefix=/usr
make clean
# Temporary workaround for a bug.
curl https://raw.githubusercontent.com/wedesoft/anymeal/master/anymeal/up.svg -o anymeal/up.svg
curl https://raw.githubusercontent.com/wedesoft/anymeal/master/anymeal/down.svg -o anymeal/down.svg
make all
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

## GNU/Linux (using AppImage)

Download the \*.AppImage file of the [latest release][2].
You just need to make the software executable:

```
chmod a+x anymeal-*-x86_64.AppImage
```

You can then run anymeal:

```
./anymeal-*-x86_64.AppImage
```

### Deinstallation

Simply delete the app image.

```
rm anymeal-*-x86_64.AppImage
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

## AnyMeal Links

* [Github](https://github.com/wedesoft/anymeal)
* [Bitbucket](https://bitbucket.org/wedesoft/anymeal/)
* [Gitlab](https://gitlab.com/wedesoft/anymeal)
* [Sourceforge](https://sourceforge.net/projects/anymeal/)
* [KDE Store](https://store.kde.org/p/1126368/)
* [GNOME Look](https://www.gnome-look.org/p/1126368)
* [Pling](https://www.pling.com/p/1126368/)
* [FSF Directory](https://directory.fsf.org/wiki/Anymeal)
* [Hackernews](https://news.ycombinator.com/item?id=23738543)
* [Arch Linux](https://aur.archlinux.org/packages/anymeal/)
* [AlternativeTo](https://alternativeto.net/software/anymeal/)
* [Softpedia](https://www.softpedia.com/get/Others/Home-Education/AnyMeal.shtml)
* [CNET Download](https://download.cnet.com/AnyMeal/3000-2126_4-78617151.html)
* [Debian Package Tracker](https://tracker.debian.org/pkg/anymeal)
* [01Net](https://www.01net.com/telecharger/windows/Loisirs/cuisine_et_boisson/fiches/153986.html)

[1]: https://wedesoft.github.io/anymeal/
[2]: https://github.com/wedesoft/anymeal/releases/latest
