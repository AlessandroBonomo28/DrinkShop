package com.uninaproject.juicylemon.components;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.EditText;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.widget.AppCompatEditText;
import androidx.core.content.res.ResourcesCompat;

import com.uninaproject.juicylemon.R;

import java.util.Objects;

public class EditTextWithClear extends androidx.appcompat.widget.AppCompatEditText {

    Drawable mClearButtonImage;

    private void initButtonImage() {
        mClearButtonImage = ResourcesCompat.getDrawable(getResources(), R.drawable.ic_clear_opaque_24dp, null);

        setCompoundDrawablesRelativeWithIntrinsicBounds(null, null, mClearButtonImage, null);

        AppCompatEditText self = this;

        setOnKeyListener((v, keyCode, event) -> {
            if (Objects.requireNonNull(getText()).length() > 0) {
                System.out.println("length > 0");
                mClearButtonImage = ResourcesCompat.getDrawable(getResources(), R.drawable.ic_clear_black_24dp, null);
            }
            else {
                mClearButtonImage = ResourcesCompat.getDrawable(getResources(), R.drawable.ic_clear_opaque_24dp, null);
            }
            invalidate();
            return false;
        });

        setOnTouchListener((v, event) -> {
            final int DRAWABLE_RIGHT = 2;
            if (event.getAction() == MotionEvent.ACTION_UP) {
                if (event.getRawX() >= (self.getRight() - self.getCompoundDrawables()[DRAWABLE_RIGHT].getBounds().width())) {

                    performClick();
                    setText("");
                    return true;
                }
            }
            return false;

        });
    }

    public EditTextWithClear(@NonNull Context context) {
        super(context);
        initButtonImage();
    }

    public EditTextWithClear(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initButtonImage();
    }

    public EditTextWithClear(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initButtonImage();
    }


}
