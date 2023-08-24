package com.uninaproject.juicylemon.events;

import android.graphics.Bitmap;

public class DrinkImageEvent {
    public Bitmap image;

    public int id;

    public DrinkImageEvent(Bitmap image, int id) {
        this.image = image;
        this.id = id;
    }
}
