# [AnyMeal][1] [![Build Status](https://github.com/wedesoft/anymeal/actions/workflows/anymeal.yml/badge.svg)](https://github.com/wedesoft/anymeal/actions/workflows/anymeal.yml) [![Downloads](https://img.shields.io/github/downloads/wedesoft/anymeal/total.svg)](https://github.com/wedesoft/anymeal/releases/latest) [![Donate](https://img.shields.io/badge/donate-The%20Water%20Project-green)](https://thewaterproject.org/)

AnyMeal is a free and open source recipe management software developed using SQLite3 and Qt6.
It can manage a cookbook with more than 250,000 MealMaster recipes, thereby allowing to import, export, search, display, edit, and print them.
AnyMeal is available for GNU/Linux and Microsoft Windows.

For Mealmaster recipes to download see:
* https://wedesoft.github.io/anymeal/

## Install from source
### Install latest release on Debian based distribution

Download the \*.tar.xz file of the [latest release][2].
You can then install the software under GNU/Linux as follows:

```Shell
tar xJf anymeal-*.tar.xz
cd anymeal-*/
sudo apt-get install build-essential autoconf libtool flex googletest libsqlite3-dev qt6-base-dev-tools qt6-tools-dev-tools qt6-base-dev qt6-svg-dev
./configure --prefix=/usr
make clean
make all
sudo make install
cd ..
```

You can then run anymeal:

```Shell
anymeal
```

### Install current software from Git repository (Debian based)

Installation from Git repository requires additional installation of autoconf and creation of the configure script:

```Shell
git clone https://github.com/wedesoft/anymeal.git
cd anymeal
sudo apt-get install build-essential autoconf libtool flex googletest libsqlite3-dev qt6-base-dev-tools qt6-tools-dev-tools qt6-base-dev qt6-svg-dev
sh ./autogen.sh
./configure --prefix=/usr
make clean
make all
sudo make install
cd ..
```

### Deinstallation

```
cd anymeal*/
sudo make uninstall
cd ..
```

The database can be removed as follows:

```
rm $HOME/.local/share/anymeal/anymeal.sqlite
```

## Install on GNU/Linux using AppImage
### Installation

Download the \*.AppImage file of the [latest release][2].
After downloading you just need to make the software executable:
Right-click on AppImage file, open preferences, and enable executable permission.

You can then run anymeal by left-clicking on the AppImage file.

### Deinstallation

Simply delete the app image.

The database can be removed by navigating to /home/\<USER\>/.local/share/anymeal and then deleting the anymeal.sqlite file.

## Install on GNU/Linux using Snap
### Installation

```
sudo snap install anymeal
```

### Deinstallation

```
sudo snap remove anymeal
```

To remove the database as well, do this instead:
```
sudo snap remove --purge anymeal
```

## Use installer for Microsoft Windows
### Installation

Download and run the \*.exe installer of the [latest release][2].
By default the installer will create a Start Menu shortcut and a desktop shortcut.

### Deinstallation

Open the Control Panel and select the applet for Programs and Features.
Select AnyMeal, then right-click and select Uninstall to remove the program.

To remove the database open the file explorer and open C:\\Users\\\<USER\>\\AppData\\Roaming\\anymeal.
Then select the anymeal.sqlite file, right-click, and choose Delete from the pop-up menu.

## Packaging status

[![Packaging status](https://repology.org/badge/vertical-allrepos/anymeal.svg)](https://repology.org/project/anymeal/versions)

## AnyMeal Links

* [Github](https://github.com/wedesoft/anymeal)
* [Bitbucket](https://bitbucket.org/wedesoft/anymeal/)
* [Gitlab](https://gitlab.com/wedesoft/anymeal)
* [Snap package](https://snapcraft.io/anymeal)
* [Sourceforge](https://sourceforge.net/projects/anymeal/)
* [KDE Store](https://store.kde.org/p/1126368/)
* [GNOME Look](https://www.gnome-look.org/p/1126368)
* [Linux Apps](https://www.linux-apps.com/p/1126368)
* [Pling](https://www.pling.com/p/1126368/)
* [FSF Directory](https://directory.fsf.org/wiki/Anymeal)
* [Hackernews](https://news.ycombinator.com/item?id=23738543)
* [Fedora Linux](https://packages.fedoraproject.org/pkgs/anymeal/)
* [AlternativeTo](https://alternativeto.net/software/anymeal/)
* [Softpedia](https://www.softpedia.com/get/Others/Home-Education/AnyMeal.shtml)
* [CNET Download](https://download.cnet.com/AnyMeal/3000-2126_4-78617151.html)
* [Debian Package Tracker](https://tracker.debian.org/pkg/anymeal)
* [Archlinux user repository page](https://aur.archlinux.org/packages/anymeal/)
* [Repology](https://repology.org/project/anymeal/versions)
* [POPCON](https://qa.debian.org/popcon.php?package=anymeal)

[1]: https://wedesoft.github.io/anymeal/
[2]: https://github.com/wedesoft/anymeal/releases/latest
