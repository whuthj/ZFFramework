/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFUIKit;

import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.Map;
import com.ZFFramework.Android.ZFCore.ZFMainEntry;
import com.ZFFramework.Android.ZFUIKit.NativeType.ZFAndroidRect;
import com.ZFFramework.Android.ZFUIKit.NativeType.ZFAndroidSize;
import android.app.Activity;
import android.graphics.Rect;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;

public class ZFUIOnScreenKeyboardState {
    private static class _WindowData {
        public boolean keyboardStateDelaying = true;
        public final ZFAndroidRect keyboardFrame = new ZFAndroidRect();
    }
    private static Map<Window, _WindowData> _keyboardState = new HashMap<Window, _WindowData>();

    private static Handler _keyboardStateUpdater = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Window window = (Window)msg.obj;
            _WindowData windowData = _keyboardState.get(window);
            if(windowData != null) {
                windowData.keyboardStateDelaying = false;
                ZFUIOnScreenKeyboardState.notifyKeyboardStateOnChange(window);
            }
        }
    };
    private static Map<Activity, _OnGlobalLayoutListener> _keyboardStateRegisterFlag = new HashMap<Activity, _OnGlobalLayoutListener>();
    private static class _OnGlobalLayoutListener implements OnGlobalLayoutListener {
        private WeakReference<Activity> _activity = null;
        private WeakReference<View> _activityRootView = null;
        public _OnGlobalLayoutListener(Activity activity, View activityRootView) {
            _activity = new WeakReference<Activity>(activity);
            _activityRootView = new WeakReference<View>(activityRootView);
        }
        private int _heightDiffOld = 0;
        @Override
        public void onGlobalLayout() {
            Window window = _activity.get().getWindow();
            _WindowData windowData = _keyboardState.get(window);
            if(windowData == null) {
                windowData = new _WindowData();
                _keyboardState.put(window, windowData);
            }
            else {
                windowData.keyboardStateDelaying = true;
            }

            Rect r = new Rect();
            _activityRootView.get().getWindowVisibleDisplayFrame(r);

            int heightDiff = _activityRootView.get().getRootView().getHeight() - (r.bottom - r.top);
            if((heightDiff >= ZFMainEntry.screenSize().height / 5) != (_heightDiffOld >= ZFMainEntry.screenSize().height / 5)) {
                _heightDiffOld = heightDiff;
                _keyboardStateUpdater.sendMessage(Message.obtain(_keyboardStateUpdater, 0, window));
            }
        }
    };
    public static void keyboardStateRegister(Activity activity, final View activityRootView) {
        _OnGlobalLayoutListener listener = new _OnGlobalLayoutListener(activity, activityRootView);
        _keyboardStateRegisterFlag.put(activity, listener);
        activityRootView.getViewTreeObserver().addOnGlobalLayoutListener(listener);
        _keyboardState.put(activity.getWindow(), new _WindowData());
    }
    public static void keyboardStateUnregister(Activity activity, View activityRootView) {
        OnGlobalLayoutListener listener = _keyboardStateRegisterFlag.remove(activity);
        activityRootView.getViewTreeObserver().removeOnGlobalLayoutListener(listener);
        _keyboardState.remove(activity.getWindow());
    }

    public static boolean native_keyboardShowing() {
        return (_keyboardFrame.height > 0);
    }

    private static ZFAndroidRect _keyboardFrame = new ZFAndroidRect();
    public static Object native_keyboardFrame() {
        return _keyboardFrame;
    }

    /**
     * @brief for special conditions, such as ZFFramework's view tree was embeded to native view,
     *   you should manually notify keyboard changing state
     */
    public static void notifyKeyboardStateOnChange(Window window) {
        if(window == null) {
            _keyboardFrame.set(0, 0, 0, 0);
            return ;
        }
        _WindowData windowData = _keyboardState.get(window);
        if(windowData == null) {
            int old = _keyboardFrame.height;
            keyboardFrameUpdate(window, _keyboardFrame);
            if(_keyboardFrame.height != old) {
                ZFUIOnScreenKeyboardState.native_notifyKeyboardStateOnChange();
            }
        }
        else if(windowData.keyboardStateDelaying) {
            _keyboardFrame.set(windowData.keyboardFrame);
        }
        else {
            int old = windowData.keyboardFrame.height;
            keyboardFrameUpdate(window, windowData.keyboardFrame);
            _keyboardFrame.set(windowData.keyboardFrame);
            if(_keyboardFrame.height != old) {
                ZFUIOnScreenKeyboardState.native_notifyKeyboardStateOnChange();
            }
        }
    }
    public static void keyboardFrameUpdate(Window window, ZFAndroidRect outKeyboardFrame) {
        Rect rect = new Rect();
        window.getDecorView().getWindowVisibleDisplayFrame(rect);
        ZFAndroidSize screenSize = ZFMainEntry.screenSize();
        outKeyboardFrame.set(0, rect.bottom, screenSize.width,
            screenSize.height - rect.bottom);
    }
    private static native void native_notifyKeyboardStateOnChange();
}
