function answer = find_thetas(l1, l2, x, y)
syms theta1 theta2
eq1 = l2*cos(theta1+theta2)+l1*cos(theta1) == x
eq2 = l2*sin(theta1+theta2)+l1*sin(theta1) == y
answer = solve(eq1,eq2, theta1, theta2)
end
