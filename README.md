# Raspberry Pi Zero 2 W Deauth Attack Detector

This is a program for Raspberry Pi Zero 2 W (and other Pi models)

<img src="https://github.com/opisite/deauth_atck_detector/assets/75821261/e6c4ecda-2bc9-47d1-b20e-263a07e42571" alt="defender_image" width="300"/>

The algorithm is simple:
  1. Sniffer thread captures management frames
  2. Upon capturing a deauth frame, push the target address to a queue
  3. detect_worker thread pops the address from the queue and adds it to the registry. If the registry contains this address as a key, the TTL value for this key is reset to TIME_TO_LIVE. If the key doesn't exist, a new key and entry are added to the registry, where the TTL value is set to TIME_TO_LIVE and startTime for the entry is set to the current time
  4. registryManager thread wakes up once per second, decrementing the TTL values of each entry
  5. registryManager checks each key to see how long it has existed in the registry. If the key has existed longer than the threshold (indicated by ( now - startTime > EXIST_THRESHOLD ), then the device alerts to an attack

