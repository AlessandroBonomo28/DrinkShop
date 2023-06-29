package com.uninaproject.juicylemon.daos;


import com.auth0.android.jwt.JWT;
public class TokenPayload {
    public String email;
    public int id;
    public String expire;
    public TokenPayload(String token) throws Exception {
        JWT jwt = new JWT(token);

        String id = jwt.getClaim("id").asString();
        String email = jwt.getClaim("email").asString();
        String expire = jwt.getClaim("expire").asString();

        if(id == null || email == null || expire == null)
            throw new Exception("Error");

        this.id = Integer.parseInt(id);
        this.email = email;
        this.expire = expire;
    }
}
