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
# 500 Internal Server Error (quantità <0 oppure id drink inesistente
# 401 Unauthorized
```
