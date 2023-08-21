package com.uninaproject.juicylemon.events;

import android.graphics.Bitmap;

public class DrinkImageEvent {
    public Bitmap image;

    public DrinkImageEvent(Bitmap image) {
        this.image = image;
    }
}
