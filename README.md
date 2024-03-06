# gtkmm-helper
Create gtkmm projects easily from the console.

[Installation](#installation)

[Utilisation](#utilisation)

## Installation
#### Clone the repository
`$ git clone https://github.com/arencoskun/gtkmm-helper`
#### Build from source
`$ mkdir build`

`$ cd build`

`$ cmake ..`

`$ make`
#### Install globally
`$ make install` or `$ sudo make install`

## Utilisation
```
allowed options:
  -h [ --help ]         produce help message
  -n [ --new ] arg      create new project
  -d [ --dir ] arg      set target directory for new project. if not set, the 
                        project will be created in the current directory.
```
#### Create project using CMake
`$ gtkmm-helper -n [project name]` or `$ gtkmm-helper -n [project name] -d [project directory]`

Thanks for using gtkmm-helper!
