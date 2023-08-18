package com.uninaproject.juicylemon.daos;

import com.uninaproject.juicylemon.lemonExceptions.UserException;
import com.uninaproject.juicylemon.model.User;

public class UserDAO implements IUserDAO {
    @Override
    public TokenPayload login(String email, String password) throws UserException {

        // TODO solleva eccezione se l'utente non esiste
        if(email.isEmpty() || password.isEmpty())
            throw new UserException("Utente inesistente");

        // TODO fetch token da API
        String token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlbWFpbCI6Im1hcmlvLnJvc3NpQGdtYWlsLmNvbSIsImV4cGlyZSI6IjIwMjMtMDgtMTggMjI6MzY6MjQiLCJpZCI6M30.YkFB39Xal1XnneAojfs3DLqEaZ5gkyq-zl__Ed68x8M";
        try{
            return new TokenPayload(token);
        } catch (Exception e){
            throw new UserException("Fail to decode token");
        }

    }

    @Override
    public User register(String email, String password) throws UserException {
        return null; // TODO
    }
}
