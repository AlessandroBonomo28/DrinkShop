# API documentation 🍸
Questa è la documentazione delle API del backend server in C. [Torna al README](../README.md)
 ```
# snippet di codice preso da router.c
# 'requireAuth' è il middleware che verifica il jwt bearer token

{ "POST", "/login", NO_MIDDLEWARE, loginHandler },
{ "POST", "/register", NO_MIDDLEWARE, registerHandler },
{ "GET", "/", requiresAuth, homeHandler },
{ "GET", "/user/:email", NO_MIDDLEWARE, getUserHandler }, 
{ "GET", "/drink/image/:id", NO_MIDDLEWARE, getDrinkImageHandler },
{ "GET", "/drink/:id", NO_MIDDLEWARE, getDrinkHandler },
{ "GET", "/drinks", NO_MIDDLEWARE, getDrinksHandler },
{ "POST", "/order/drink", requiresAuth, orderDrinkHandler },
{ "PUT", "/order/drink", requiresAuth, updateDrinkQuantityInOrderHandler },
{ "DELETE", "/order", requiresAuth, deleteOrderHandler },
{ "DELETE", "/order/drink/:id", requiresAuth, deleteDrinkFromOrderHandler},
{ "GET", "/order/last", requiresAuth, getLastOrderMadeByUserHandler },
{ "GET", "/orders", requiresAuth, getOrdersMadeByUserHandler },
{ "GET", "/order/:id", requiresAuth, getOrderHandler },
{ "POST", "/pay", requiresAuth, payOrderHandler }
 ```
 #### Indice 📖
[login](##-POST-/login)  
[register](##-POST-/register) 
## POST /login
- Serve a autenticare l'utente, se ha successo, restituisce un token jwt
- Non richiede autorizzazione
### Request example
```
# Content type: application/json

{
    "email":"alex@gmail.com",
    "password":"123"
}
```
### Responses
```
# 200 OK
{
    "token":"xxxx"
}

# 400 Bad request
# 401 Unauthorized
```
## POST /register
- Serve a creare un nuovo utente
- Non richiede autorizzazione
### Request example
```
# Content type: application/json

{
    "email":"alex@gmail.com",
    "password":"123"
}
```
### Responses
```
# 200 OK
# 400 Bad request
# 500 Internal Server Error (utente già esistente)
```
## GET /
- Home dell'utente con messaggio di benvenuto
- Richiede autorizzazione
### Request example
```
# Header authorization: Bearer token
```
### Responses
```
# 200 OK
body: <messaggio di benvenuto>

# 401 Unauthorized
```
## GET /user/:email
- Restituisce le informazioni di un utente data la usa email
- Non richiede autorizzazione
### Request example
```
URL parameter email: email dell'utente 
URL request example: /user/alex@gmail.com
```
### Responses
```
# 200 OK
{
    "email": "alex@gmail.com",
    "id": 1
}

# 404 Not found (utente inesistente)
```
## GET /drink/image/:id
- Restituisce l'immagine di un drink dato il suo id
- Non richiede autorizzazione
### Request example
```
URL parameter id: id del drink
URL request example: /drink/image/1
```
### Responses
```
# 200 OK
body: file immagine (jpg, jpeg, png ...)

# 404 Not found (drink inesistente)
```
## GET /drink/:id
- Restituisce informazioni sul drink dato il suo id
- Non richiede autorizzazione
### Request example
```
URL parameter id: id del drink
URL request example: /drink/1
```
### Responses
```
# 200 OK

{
    "name": "Negroni",
    "description": "Cocktail amaro e intenso",
    "price": 2.5,
    "id": 1,
    "image_url": "images/drinks/negroni.jpg"
}
# 404 Not found (drink inesistente)
```
## GET /drinks
- Restituisce tutti i drink disponibili
- Non richiede autorizzazione
### Request example
```
URL request example: /drinks
```
### Responses
```
# 200 OK
[
    {
        "id": "1",
        "name": "Negroni",
        "description": "Cocktail amaro e intenso",
        "image_url": "images/drinks/negroni.jpg",
        "price": "2.5"
    },
    {
        "id": "2",
        "name": "Long Island",
        "description": "Cocktail forte e deciso",
        "image_url": "images/drinks/longisland.jpg",
        "price": "3"
    }
]
# 500 Internal Server Error
```
## POST /order/drink
- Permette di ordinare un drink
- Richiede autorizzazione

*Nota: Il drink verrà aggiunto automaticamente all'ordine in corso (se non esiste viene creato un nuovo ordine), se esiste già un ordinazione dello stesso drink, la quantità verrà sommata.*
### Request example
```
# Header authorization: Bearer token
# Content type: application/json

{
    "quantity":1,
    "id_drink":2
}
```
### Responses
```
# 200 OK
# 400 Bad request
# 500 Internal Server Error (quantità <0 o id drink inesistente o nessun ordine trovato)
# 401 Unauthorized
```
## PUT /order/drink
- Permette di aggiornare la quantità di un drink in un ordine in corso
- Richiede autorizzazione
### Request example
```
# Header authorization: Bearer token
# Content type: application/json

{
    "quantity":1,
    "id_drink":2
}
```
### Responses
```
# 200 OK
# 400 Bad request
# 500 Internal Server Error (quantità <0 o id drink inesistente o nessun ordine trovato)
# 401 Unauthorized
```
## DELETE /order
- Permette di cancellare l'ordine in corso
- Richiede autorizzazione
### Request example
```
# Header authorization: Bearer token
```
### Responses
```
# 200 OK
# 500 Internal Server Error (Nessun ordine trovato)
# 401 Unauthorized
```
## DELETE /order/drink/:id
- Permette di cancellare un drink dall'ordine in corso
- Richiede autorizzazione
### Request example
```
# Header authorization: Bearer token
URL parameter: id del drink da cancellare
URL request example: /order/drink/1
```
### Responses
```
# 200 OK
# 500 Internal Server Error (Nessun ordine trovato o drink inesistente)
# 401 Unauthorized
```
## GET /order/last
- Permette di ottenere informazioni sull'ordine più recente
- Richiede autorizzazione
### Request example
```
# Header authorization: Bearer token
```
### Responses
```
# 200 OK
{
    "drinks": [
        {
            "id": "1",
            "id_order": "1",
            "id_item": "1",
            "quantity": "2",
            "id_drink": "1",
            "name": "Negroni",
            "description": "Cocktail amaro e intenso",
            "image_url": "images/drinks/negroni.jpg",
            "price": "2.5"
        },
        {
            "id": "2",
            "id_order": "1",
            "id_item": "2",
            "quantity": "10",
            "id_drink": "2",
            "name": "Long Island",
            "description": "Cocktail forte e deciso",
            "image_url": "images/drinks/longisland.jpg",
            "price": "3"
        }
    ],
    "order": {
        "id": 1,
        "id_user": 1,
        "creation_datetime": "2023-05-26 20:41:57.865745",
        "paid": true
    },
    "total": 35.000000
}
# 404 Not Found (Nessun ordine trovato)
# 401 Unauthorized
```
## GET /orders
- Permette di ottenere informazioni su ordini effettuati in passato
- Richiede autorizzazione

*Nota: Restituisce gli ultimi 10 ordini, se gli ordini sono contrassegnati come 'paid: t' allora sono ordini pagati e hanno i dati del pagamento. Se un ordine è contrassegnato con 'paid: f' allora si tratta di un ordine in corso che non è ancora stato pagato e di conseguenza ha i dati di pagamento vuoti.*
### Request example
```
# Header authorization: Bearer token
```
### Responses
```
# 200 OK
[
    {
        "id": "2",
        "id_user": "1",
        "creation_timestamp": "2023-05-27 17:06:26.910592",
        "paid": "f",
        "id_payment": "",
        "card_holder": "",
        "card_number": "",
        "CVV": "",
        "expiration_date": "",
        "payment_timestamp": "",
        "amount": ""
    },
    {
        "id": "1",
        "id_user": "1",
        "creation_timestamp": "2023-05-26 20:41:57.865745",
        "paid": "t",
        "id_payment": "1",
        "card_holder": "me",
        "card_number": "123",
        "CVV": "45",
        "expiration_date": "01/21",
        "payment_timestamp": "2023-05-26 22:02:00.585581",
        "amount": "2.5"
    }
]
# 404 Not Found (Nessun ordine trovato)
# 401 Unauthorized
```
## GET /order/:id
- Permette di ottenere informazioni sull'ordine dal suo id
- Richiede autorizzazione
### Request example
```
# Header authorization: Bearer token
URL parameter: id dell'ordine
URL request example: /order/1
```
### Responses
```
# 200 OK
{
    "drinks": [
        {
            "id": "1",
            "id_order": "1",
            "id_item": "1",
            "quantity": "2",
            "id_drink": "1",
            "name": "Negroni",
            "description": "Cocktail amaro e intenso",
            "image_url": "images/drinks/negroni.jpg",
            "price": "2.5"
        },
        {
            "id": "2",
            "id_order": "1",
            "id_item": "2",
            "quantity": "10",
            "id_drink": "2",
            "name": "Long Island",
            "description": "Cocktail forte e deciso",
            "image_url": "images/drinks/longisland.jpg",
            "price": "3"
        }
    ],
    "order": {
        "id": 1,
        "id_user": 1,
        "creation_datetime": "2023-05-26 20:41:57.865745",
        "paid": true
    },
    "total": 35.000000
}
# 404 Not Found (Nessun ordine trovato)
# 403 Forbidden (Non puoi visualizzare l'ordine di qualcun'altro)
# 401 Unauthorized
```
## POST /pay
- Permette di pagare l'ordine in corso
- Richiede autorizzazione

*Nota: viene controllato automaticamente se l'importo è >= del totale dell'ordine. Tutto ciò che è > del totale viene considerato mancia.*
### Request example
```
# Header authorization: Bearer token
# Content Type: application/json
{
    "amount":2.50,
    "card_holder": "alex",
    "card_number": "1234567891112130",
    "CVV":"123",
    "expiration_date":"01/21"
}
```
### Responses
```
# 200 OK
# 500 Internal Server Error (Nessun ordine da pagare o importo non sufficiente)
# 400 Bad Request
# 401 Unauthorized
```
