# ใช้ภาพพื้นฐานของ Ubuntu
FROM debian:latest

# อัปเดตแพ็กเกจและติดตั้งโปรแกรม เช่น curl และ vim
RUN apt-get update && apt-get install -y \
    g++ \
    php-cgi \
    cmake \
    curl \
    vim \
    lsof \
    && rm -rf /var/lib/apt/lists/*

# กำหนดให้ใช้ bash เป็น shell เริ่มต้น
CMD ["bash"]
