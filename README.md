# AnyMeal [![Build Status](https://travis-ci.org/wedesoft/anymeal.svg?branch=master)](https://travis-ci.org/wedesoft/anymeal)

## Installation

Download [anymeal-master.zip][3] and then install the software as follows:

```Shell
unzip anymeal-master.zip
cd anymeal-master
sudo apt-get install build-essential autoconf automake flex googletest librecode-dev libsqlite3-dev qt5-default libqt5opengl5-dev
./autogen.sh
./configure
make
sudo make install
cd ..
```

You can then run anymeal:

```Shell
anymeal
```

You can download a lot of zipped MealMaster recipes for importing from

* [Glen's MealMaster recipe archive][1]
* [NYC recipes][2].

[1]: http://thehoseys.org/buster/recipes.html
[2]: http://www.ffts.com/recipes.htm
[3]: https://github.com/wedesoft/anymeal/archive/master.zip
