# cbox - simple containers in C
Why? For fun :P

## Building
```bash
mkdir build
cd build
cmake ../
cmake --build . -j4
```

## Running
In project directory create container environment (docker should be installed)
```bash
./scripts/busybox_create.sh test
```
after that folder `runtime/test` will be created with container content

Now you are ready to run this!
```bash
cd build
sudo ./runner
```
after this commands shell will be executed in isolated container
