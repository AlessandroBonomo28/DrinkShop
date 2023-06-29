package com.uninaproject.juicylemon.daos;

import com.uninaproject.juicylemon.lemonExceptions.UserException;

public interface IUserDAO {
    public TokenPayload login(String email, String password) throws UserException;
}
