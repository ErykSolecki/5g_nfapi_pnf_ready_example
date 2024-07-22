# 5G nFapi simple client-server example
Simple client and server application to send PNF_READY.indication nFapi message via sctp.

## How to run vnf_receiver:

```
cd vnf_receiver
pip install -r requirements.txt
python3 vnf_receiver.py
```

## How to build and run pnf_sender:

To use cmake presets Ninja build system is required:

`sudo apt-get install ninja-build`

Instal sctp library:

`sudo apt install libsctp-dev`

Build release from cmake preset:
```
cd pnf_sender/
cmake --preset release
cmake --build --preset release
```

Run executable:

`./build/release/pnf_sender`