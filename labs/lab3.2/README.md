
Compile
--------
```
make
```

Encoding
--------
```
./base64 --encode input.txt 
or
./base64.out --encode input.txt 
```
Generates a file `encoded.txt` with the contents from `input.txt` encoded

Decoding
--------
```
./base64 --decode encoded.txt
or
./base64.out --decode encoded.txt
```
Generates a file `decoded.txt` with the contents from `input.txt` decoded

How to test?
------------
- Get process ID
```
ps aux | grep base64
```

- Send the signals
```
kill -SIGINT <PID>
kill -SIGINFO <PID>
```