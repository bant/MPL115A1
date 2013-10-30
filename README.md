MPL115A1
========

Raspberry PiにてSPI版大気圧センサMPL1151から、気圧・温度を読み込むコマンド


構築&インストール方法
--------------------
構築方法は以下のとおり

`$>make`

インストール方法は以下のとおり

`$>sudo make install`

これにて /usr/local/bin にMPL115A1コマンドがインストールされます。


アンインストール方法は以下のとおり

`s>sudo make uninstall`

なお、本コマンドはwiringPiライブラリィを利用しているので、構築にはwiringPiライブラリィがraspberry Piにインストールされいいる必要があります。

wiringPiの詳細は、以下のところを参照してください。
https://projects.drogon.net/raspberry-pi/wiringpi/
