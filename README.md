# Drink Shop 🍹
Drink shop con architettura 3 tier client-server-data. Android Java client + C Http Socket Server + Postgres DB. 

Per tutti i dettagli implementativi [Download documentation](https://github.com/AlessandroBonomo28/DrinkShop/raw/main/Documentazione%20Juicy%20Lemon.docx)
 
Per informazioni sulle API del server in C vedi [API docs](docs/routes.md)

 Il server è dockerizzato e ha due container:
 - postgres-db
 - server-c

prima di fare il *compose dei container* assicurarsi che il server non sia configurato in debug mode. Controllare che la stringa di connessione al database *PRODUCTION CONFIGURATION* non sia commentata.
 ```
 // Configurazione del file server.h
 
 // PRODUCTION CONFIGURATION 
 // #define DATABASE "host=postgres-db port=5432 dbname=drinks user=docker password=12345"

 // DEBUG CONFIGURATION 
 //#define DATABASE "host=127.0.0.1 port=42069 dbname=drinks user=docker password=12345"
 ```
 Nel caso fosse commentata basterà commentare la *DEBUG CONFIGURATION*, decommentare *PRODUCTION CONFIGURATION*, eseguire
 ```
# installare librerie se non presenti
# installa libreria postgres
RUN apt-get update && apt-get install -y libpq-dev
# installa libreria json
RUN apt-get install libjson-c-dev -y
# installa libreria jwt token
RUN sudo apt-get install libjwt-dev -y
# installa libreria bcrypt
sudo apt-get install libcrypt-dev -y

cd <directory server>

./build.sh  # compilo server
 
cd <root progetto>
# ! se hai una vecchia immagine del progetto in cache di docker eliminala
docker compose up
 ```
 # Features del server
 - Elabora richieste Http GET, POST, PUT, DELETE
 - Parsing di richieste Http con content application/json
 - Risposte Http con content application/json
 - Smart path routing con URL parameter (esempio: /user, /user/:id) 
 - Può includere files nella risposta Http
 - Jwt bearer token authentication
 - Jwt token expires after custom amount of time
 - Password encryption
 - Models per interfacciamento con tabelle del database
 - Possibilità di eseguire custom middleware per ogni route
 - Handler per ogni route
 - Custom routes prefix
 - Multithreading
 - Pool di connessioni con custom max clients  
 - Custom socket timeout
 - Gestione interrupt sigterm di terminazione (esempio CTRL+C)
 - Messaggi di stato del server sulle connessioni attive e eventi
 ### 404 Server custom response
 Quando il router non trova la route richiesta serve il client con un codice 404 e questa immagine:

![egg](https://github.com/AlessandroBonomo28/DrinkShop/assets/75626033/f99197cd-edd3-4355-9ca0-2bce1d719721)

 ## Come si potrebbe migliorare
 - IP spam filter
 - Incomplete request detection
 - check for mem leaks 💀

### Note finali
Progetto presentato il 9 ottobre 2023.
