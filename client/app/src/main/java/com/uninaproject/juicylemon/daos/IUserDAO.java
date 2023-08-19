package com.uninaproject.juicylemon.daos;

import android.content.Context;

import com.uninaproject.juicylemon.lemonExceptions.UserException;
import com.uninaproject.juicylemon.model.User;
public interface IUserDAO {
    public TokenPayload login(String email, String password, Context context) throws UserException;
    public void register(String email, String password, Context context) throws UserException;
}
