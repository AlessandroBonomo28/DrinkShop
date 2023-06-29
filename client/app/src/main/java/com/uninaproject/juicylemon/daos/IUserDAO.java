package com.uninaproject.juicylemon.daos;

import com.uninaproject.juicylemon.lemonExceptions.UserException;
import com.uninaproject.juicylemon.model.User;
public interface IUserDAO {
    public TokenPayload login(String email, String password) throws UserException;
    public User register(String email, String password) throws UserException;
}
