package com.uninaproject.juicylemon.components;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.text.InputType;
import android.util.AttributeSet;
import android.view.MotionEvent;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.widget.AppCompatEditText;
import androidx.core.content.res.ResourcesCompat;

import com.uninaproject.juicylemon.R;

public class EditTextWithShowHideButton extends androidx.appcompat.widget.AppCompatEditText {

    Drawable mShowHideButtonImage;

    private boolean isShowingPassword = false;

    private void initButtonImage() {
        mShowHideButtonImage = ResourcesCompat.getDrawable(getResources(), R.drawable.ic_show_hide_eye_24, null);

        setCompoundDrawablesRelativeWithIntrinsicBounds(null, null, mShowHideButtonImage, null);

        AppCompatEditText self = this;

        setOnTouchListener((v, event) -> {
            final int DRAWABLE_RIGHT = 2;
            if (event.getAction() == MotionEvent.ACTION_UP) {
                if (event.getRawX() >= (self.getRight() - self.getPaddingRight() - self.getCompoundDrawables()[DRAWABLE_RIGHT].getBounds().width())) {

                    performClick();
                    if (isShowingPassword)
                        setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_VISIBLE_PASSWORD);
                    else
                        setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);

                    isShowingPassword = !isShowingPassword;

                    return true;
                }
            }
            return false;

        });
    }

    public EditTextWithShowHideButton(@NonNull Context context) {
        super(context);
        initButtonImage();
    }

    public EditTextWithShowHideButton(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initButtonImage();
    }

    public EditTextWithShowHideButton(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initButtonImage();
    }


}
