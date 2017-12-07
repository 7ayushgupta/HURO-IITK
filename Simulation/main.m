function main(l1, l2, x,y)
syms x0 answer1 x2 y2 r min max
r = (x*x + y*y)^.5
x0 = l1 + l2
max = x0 - r
min = r - l2 + l1
if(max>=0 && min>=0)
    hold on
    axis([-10 10 -10 10])
    plot([l1+l2 x],[0 y], 'bx')
    plot([0 l1],[0 0])
    plot([l1 x0],[0 0])
    plot([0 l1],[0 0], 'ro')
    plot([l1 x0],[0 0],'ro')
    pause(2)
    clf
    for i = 0.05:0.05:1
        C = [x0+i*(x-x0) i*y]
        answer1 = find_thetas(l1,l2,C(1,1),C(1,2))
        [a b] = size(answer1.theta1)
        if(a>0 && b>0)
            [x2 y2] = find_xy(answer1.theta1(1,1),l1)
            axis([-10 10 -10 10])
            plotter(0,0,x2,y2,C(1,1),C(1,2),x,y)
        end
    end
else
    fprintf('Invalid')
end
end
