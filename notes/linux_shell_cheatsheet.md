# Linux Shell Cheatsheet

## tar

* tar a directory

```
    tar -zcvf archive-name.tar.gz directory-name

    -z : Compress archive using gzip program
    -c: Create archive
    -v: Verbose i.e display progress while creating archive
    -f: Archive File name
```

* untar a tarball

```
    tar -zxvf prog-1-jan-2005.tar.gz
    tar -xf prog-1-jan-2005.tar.gz # tar can detect the gzip format automatically

    -x: extract
```
