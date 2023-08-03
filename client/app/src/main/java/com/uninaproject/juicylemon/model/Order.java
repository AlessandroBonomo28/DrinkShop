package com.uninaproject.juicylemon.model;
import java.util.Date;

public class Order {
    public int id;
    public User client;
    public Date date;
    public Drink[] drinks;
    public boolean paid;

    public Order(int id, User client, Date date, Drink[] drinks, boolean paid) {
        this.id = id;
        this.client = client;
        this.date = date;
        this.drinks = drinks;
        this.paid = paid;
    }

    public float getTotal(){
        float sum = 0;
        for(Drink d : drinks){
            sum += d.price;
        }
        return sum;
    }
}
