 # Routes del server
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
## GET /login
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