#!/bin/bash
docker run -ti --rm -v "$PWD/linux":/home/student/src/linux -v "$PWD/../2/initramfs":/home/student/src/initramfs tiagoshibata/pcs3746