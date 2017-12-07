function plotter(a,b,c,d,e,f,h,g)
hold on
axis([-10 10 -10 10])
plot([a h],[b g], 'bx')
plot([a c],[b d],'ro')
plot([c e],[d f],'ro')
plot([a c],[b d])
plot([c e],[d f])
pause(.5)
clf
end
