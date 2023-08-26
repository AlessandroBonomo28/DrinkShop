# Specifica l'immagine di base
FROM gcc:latest

RUN apt-get update && \
    apt-get install -yq tzdata && \
    ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime && \
    dpkg-reconfigure -f noninteractive tzdata

ENV TZ="Europe/Rome"

# installa libreria postgres
RUN apt-get update && apt-get install -y libpq-dev
# installa libreria json
RUN apt-get install libjson-c-dev -y
# installa libreria jwt token
RUN apt-get install libjwt-dev -y
# installa libreria bcrypt
RUN apt-get install libcrypt-dev -y

# Copia il contenuto nella directory /src del container
COPY . .

RUN chmod +x build.sh
RUN ./build.sh

# Avvia il server
CMD ["./server"]
