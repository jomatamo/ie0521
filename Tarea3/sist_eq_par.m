function dydt = odefun(t,y)
dydt = zeros(2,1);
dydt(1) = y(2);
dydt(2) = -2*y(1)-2*y(2)+ heaviside(t);

end
