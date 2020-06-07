# ![](anymeal/anymeal.png) [AnyMeal][3] [![Build Status](https://travis-ci.org/wedesoft/anymeal.svg?branch=master)](https://travis-ci.org/wedesoft/anymeal)

## Installation under GNU/Linux

Download the \*.tar.xz file of the [latest release][4].
You can then install the software under GNU/Linux as follows:

```Shell
tar xJf anymeal-*.tar.xz
cd anymeal-*/
sudo apt-get install build-essential flex googletest librecode-dev libsqlite3-dev qtbase5-dev-tools qttools5-dev-tools qtbase5-dev
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
[3]: https://wedesoft.github.io/anymeal/
[4]: https://github.com/wedesoft/anymeal/releases/latest
