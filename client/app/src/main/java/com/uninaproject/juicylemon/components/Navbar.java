package com.uninaproject.juicylemon.components;

import android.content.Context;
import android.content.Intent;
import android.util.AttributeSet;
import android.widget.ImageButton;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.widget.Toolbar;
import androidx.core.content.res.ResourcesCompat;
import androidx.lifecycle.LifecycleOwner;
import androidx.lifecycle.ViewModelProvider;
import androidx.lifecycle.ViewModelStoreOwner;

import com.uninaproject.juicylemon.R;
import com.uninaproject.juicylemon.activities.CartActivity;
import com.uninaproject.juicylemon.events.CartUpdateEvent;
import com.uninaproject.juicylemon.utils.LoginManager;
import com.uninaproject.juicylemon.utils.Utils;
import com.uninaproject.juicylemon.view_models.NavbarViewModel;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;

public class Navbar extends Toolbar {

    TextView welcome_text;

    ImageButton cart_button;

    TextView cart_counter;

    NavbarViewModel viewModel;

    public Navbar(Context context) {
        super(context);
        init();
    }

    public Navbar(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public Navbar(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    void init() {
        EventBus.getDefault().register(this);

        viewModel = new ViewModelProvider((ViewModelStoreOwner) getContext()).get(NavbarViewModel.class);

        setBackground(ResourcesCompat.getDrawable(getResources(), R.drawable.box_shadow, null));

        // inflate layout
        inflate(getContext(), R.layout.navbar_dashboard, this);

        welcome_text = findViewById(R.id.navbar_text_welcome);
        cart_button = findViewById(R.id.cart_button);

        cart_counter = findViewById(R.id.cart_counter);
        initCartCounter();


        String nomeUtente = Utils.separateEmail(LoginManager.getInstance().getUser().email).first;
        welcome_text.setText(getResources().getString(R.string.welcome_text, nomeUtente));
        
        cart_button.setOnClickListener(v -> {
            Intent intent = new Intent(getContext(), CartActivity.class);
            getContext().startActivity(intent);
        });


        viewModel.getCartSize().observe((LifecycleOwner) getContext(), size -> {
            if (size == 0)
                cart_counter.setText("");
            else
                cart_counter.setText(String.valueOf(size));
        });
    }


    void initCartCounter() {
        if (viewModel.getCartSize().getValue() != null && viewModel.getCartSize().getValue() == 0) {
            cart_counter.setText("");
        } else
            cart_counter.setText(String.valueOf(viewModel.getCartSize().getValue()));
    }

    @Subscribe
    public void onCartUpdate(CartUpdateEvent event) {
        if (event.getCartSize() == 0)
            viewModel.setCartSize(0);
        else
            viewModel.setCartSize(event.getCartSize());
    }

}
