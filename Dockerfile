FROM archlinux:latest

# Обновление системы и установка базовых пакетов
RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm \
    base-devel \
    git \
    cmake \
    ninja \
    qt6-base \
    qt6-declarative \
    qt6-tools \
    qt6-svg \
    qt6-multimedia \
    qt6-webengine \
    qt6-wayland \
    qt6-shadertools \
    clang \
    lld \
    ccache \
    python \
    python-pip \
    docker \
    docker-compose \
    && pacman -Scc --noconfirm

# Создание пользователя для сборки (опционально)
RUN useradd -m -s /bin/bash builder && \
    echo "builder ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

WORKDIR /workspace
