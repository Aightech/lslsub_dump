# lslsub_dbfeeder


## Information
- **Brief**: Search for available LSL stream, for each of them create a thread doing the following: open the database, create a table and store the samples.
- **Documentation & Git**: [Doc link](https://aightech.github.io/lslsub_dump/html/index.html) & [Git link](https://github.com/Aightech/lslsub_dump)
- **Languages**: C++
- **Libraries**: LSL / pqxx
- **Note**: /
- **Compatibility**:

| Ubuntu           | Window10         | MacOS            |
|:----------------:|:----------------:|:----------------:|
|:heavy_check_mark:|:heavy_check_mark:|:grey_question:   |


## Building
### Ubuntu
#### Steps
- Clone the repository and go inside.
```bash
git clone https://github.com/Aightech/lslsub_dump.git && cd lslsub_dump
```
- Create a build directory and go inside.
- Configure the project.
- Build the project.
```bash
mkdir build && cd build && cmake .. && cmake --build .
```

**Rq:** *The exe file is called lslsub_dump. This file has also been copied in the bin floder of the git repository root.*

### Windows 10
#### **Requirements**
- [CMake](https://cmake.org/download/) (download and execute the installer for windows , add to the PATH variable)
- [MinGW32](https://sourceforge.net/projects/mingw-w64/) (download and execute the installer for windows, chose i686_64 architecture, add the the bin folder address of minGW to the PATH environement variable).
- [**Git**](https://git-for-windows.github.io/) Download and install git.
#### Steps
- Clone the repository and go inside.
```bash
git clone https://github.com/Aightech/lslsub_dump.git && cd lslsub_dump
```
- Create a build directory.
- Configure and generate the CMake project.
- Build the project.
```bash
mkdir build && cd build && cmake .. -G "MinGW Makefiles" && mingw32-make
```
**Warning:** You might encouter an issue of compilation related to thread libraries. To solve this issue modify the file that get the error and add ".p" at the and of the "__x._M_thread" like variable. ("__x._M_thread.p" and "__y._M_thread.p").

**Rq:** *The exe file is called lslsub_dump. This file has also been copied in the bin floder of the git repository root.*
