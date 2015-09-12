# MCPP - A C++ Mongrel2 client library.

## Dependencies
* libjsoncpp - https://github.com/open-source-parsers/jsoncpp
* zeromq - http://www.zeromq.org/

## How to build and install

```
git clone https://github.com/samvik/mcpp.githttps://github.com/samvik/mcpp.git
mkdir mcpp_build
cd mcpp_build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLE=On ../mcpp
make
make install
```

## License
New BSD (3-clause)
