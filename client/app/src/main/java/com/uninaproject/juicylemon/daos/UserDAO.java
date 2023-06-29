package com.uninaproject.juicylemon.daos;

import com.uninaproject.juicylemon.lemonExceptions.UserException;
import com.uninaproject.juicylemon.model.User;

public class UserDAO implements IUserDAO {
    @Override
    public TokenPayload login(String email, String password) throws UserException {

        // TODO solleva eccezione se l'utente non esiste
        if(email=="" || password =="")
            throw new UserException("Utente inesistente");

        // TODO fetch token da API
        String token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJlbWFpbCI6ImFsZXhAZ21haWwuY29tIiwiZXhwaXJlIjoiMjAyMy0wNi0yOSAxMDowNzo0NiIsImlkIjoxfQ.FZhz0g5lOtvLrvrdHRsgc0yhK36itkGdHQCYtzg1AHc";
        try{
            return new TokenPayload(token);
        } catch (Exception e){
            throw new UserException("Fail to decode token");
        }

    }

    @Override
    public User register(String email, String password) throws UserException {
        return null;
    }
}
