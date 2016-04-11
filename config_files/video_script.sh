#will install and start the video server
(cd ~
opkg install kernel-module-uvcvideo
cp IOT_Education/config_files/ffmpeg_files/ffserver.conf /etc
chmod +x IOT_Education/config_files/ffmpeg_files/install_ffmpeg.sh
./IOT_Education/config_files/ffmpeg_files/install_ffmpeg.sh
chmod +x IOT_Education/config_files/ffmpeg_files/video_server.sh
nohup ./IOT_Education/config_files/ffmpeg_files/video_server.sh &
)

