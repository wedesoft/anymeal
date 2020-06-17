# [AnyMeal][1] [![Build Status](https://travis-ci.org/wedesoft/anymeal.svg?branch=master)](https://travis-ci.org/wedesoft/anymeal) [![Downloads](https://img.shields.io/github/downloads/wedesoft/anymeal/total.svg)](https://github.com/wedesoft/anymeal/releases/latest)

## Installation under GNU/Linux

Download the \*.tar.xz file of the [latest release][2].
You can then install the software under GNU/Linux as follows:

```Shell
tar xJf anymeal-*.tar.xz
cd anymeal-*/
sudo apt-get install build-essential flex googletest librecode-dev libsqlite3-dev qt5-default qttools5-dev-tools
./configure
make
sudo make install
cd ..
```

You can then run anymeal:

```Shell
anymeal
```

[1]: https://wedesoft.github.io/anymeal/
[2]: https://github.com/wedesoft/anymeal/releases/latest
