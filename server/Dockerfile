# Specifica l'immagine di base
FROM gcc:latest

# installa libreria postgres
RUN apt-get update && apt-get install -y libpq-dev
# installa libreria json
RUN apt-get install libjson-c-dev -y

# Copia il contenuto nella directory /src del container
COPY . /src

# Imposta la directory di lavoro
WORKDIR /src

# Compila il server
RUN chmod +x build.sh
RUN ./build.sh

# Avvia il server
CMD ["./server"]