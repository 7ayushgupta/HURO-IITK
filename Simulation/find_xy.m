function [x, y] = find_xy(theta01, l1)
syms x y
x = l1*cos(theta01)
y = l1*sin(theta01)
