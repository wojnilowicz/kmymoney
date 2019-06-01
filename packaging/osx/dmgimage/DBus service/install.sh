#!/bin/bash
# Copying service to user's services location
cp -fv /Volumes/KMyMoneyNEXT/kmymoney.app/Contents/Library/LaunchAgents/org.freedesktop.dbus-session.plist ~/Library/LaunchAgents

# Starting copied service with -w switch, which forces start in case of problems
launchctl load -w ~/Library/LaunchAgents/org.freedesktop.dbus-session.plist
launchctl enable -w ~/Library/LaunchAgents/org.freedesktop.dbus-session.plist