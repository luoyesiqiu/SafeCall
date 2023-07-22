package com.luoye.safecaller;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

import com.luoye.safecaller.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        binding.btnNormalCall.setOnClickListener(new MyOnClickListener());
        binding.btnProblemCall1.setOnClickListener(new MyOnClickListener());
        binding.btnProblemCall2.setOnClickListener(new MyOnClickListener());

    }

    private static class MyOnClickListener implements View.OnClickListener {
        @Override
        public void onClick(View v) {
            if(v.getId() == R.id.btn_normal_call) {
                JNIHelper.normalCall();
            }
            else if(v.getId() == R.id.btn_problem_call1) {
                JNIHelper.problemCall1();
            }
            else if(v.getId() == R.id.btn_problem_call2) {
                JNIHelper.problemCall2();
            }

        }
    }

}