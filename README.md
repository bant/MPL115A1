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

なお、本コマンドはwiringPiライブラリィを利用しているので、構築にはwiringPiライブラリィがraspberry Piにインストールされている必要があります。

wiringPiの詳細は、以下のところを参照してください。

https://projects.drogon.net/raspberry-pi/wiringpi/

使用方法
--------
MPL115A1をRaspbery Piに接続し、

`$>MPL115A1 -c1　-t1000`

のように入力すると、以下のように大気圧と温度が得られる。

    MPL115A1 -c1 -t100
    ***** Start MPL115A1 Barometric Pressure Sensor *****
    [Contions]--------------
    SPI Channel: 1 ch
    SPI Speed: 1000000 Hz
    Average times: 100
    
    [Results]---------------
    Pressure: 1020.6 hPa
    Temperature: 16.2 degC






