# Python-Ogre (aka. PyOgre)
**Warning**: This repository is not a rebirth of PyOgre. PyOgre is dead (AFAI can tell), and I think a
_tremendous_ amount of work would be needed to raise it from its ashes.

**Note**: Official(?) statement from the (formerly official) python-ogre.org website, from December 2016
([archived version](http://web.archive.org/web/20170725202607/http://www.python-ogre.org/)):

    This blog has been dead for quite some time; but rest assured Python Ogre is still being updated – just check github.

I did check GitHub, but did not find any information on PyOgre…

I have no idea whether this statement comes from the actual maintainer of Python-Ogre. Even if it did, the project is
now clearly dead: the python-ogre.org domain has been bought and is no longer related to the project.

If the author of the post read this, or if anybody has more information, you're welcome to open an issue and clarify
the situation :)

## What is PyOgre?
As stated [here](http://www.ogre3d.org/tikiwiki/PyOgre), PyOgre is a Python interface on Ogre, which
directly calls the C++ implementation.

## What is Ogre?
[OGRE (**O**bject-oriented **G**raphics **R**endering **E**ngine) is an open-source graphics rendering engine that is written and maintained by a small core team, and contributed to by its ever growing community.](http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Getting+Started)

## Building PyOgre
I tried. It just does not work, at least not on Sierra. Pretty much all the dependencies are either obsolete or old, and making them compile is usually very tricky. After a while, I just gave up :)

Original build instructions can be found here (in the wayback machine as the original website is down):
   http://web.archive.org/web/20160418235928/http://wiki.python-ogre.org/index.php?title=OSXBuildV2

## Why This Repo?
I think the PyOgre project is a good idea. I personally don't have the time to invest in developing it, but if anyone ever wanted to dig into it, at least you now have my two cents of research available.

For what it's worth, [I'm apparently not the only one thinking this project is a good idea](https://github.com/isosphere/python-ogre).

## Cloning Method
This repo has been created with the following command line:
```
git svn clone --trunk=trunk --tags=tags --branches=branches https://svn.code.sf.net/p/python-ogre/code/ python-ogre
```
