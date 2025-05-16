with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

procedure Main is
    task TaskA is
        entry Start(Counter : in out Integer);
    end TaskA;
    
    task body TaskA is
    begin
        accept Start(Counter : in out Integer) do
            for i in 0..1000 loop
                Put_Line("TaskA");
                Counter := Counter + 1;
                delay 0.0;
            end loop;
        end Start;
    end TaskA;

    task TaskB is
        entry Start(Counter : in out Integer);
    end TaskB;
    
    task body TaskB is
    begin
        accept Start(Counter : in out Integer) do
            for i in 0..1000 loop
                Put_Line("TaskB");
                Counter := Counter + 1;
            end loop;
        end Start;
    end TaskB;

    task TaskC is
        entry Start(Counter : in out Integer);
    end TaskC;
    
    task body TaskC is
    begin
        accept Start(Counter : in out Integer) do
            for i in 0..1000 loop
                Put_Line("TaskC");
                Counter := Counter + 1;
            end loop;
        end Start;
    end TaskC;

   Counter : Integer := 0;

begin
    TaskA.Start(Counter);
    TaskB.Start(Counter);
    TaskC.Start(Counter);
    Put(Counter);
    New_Line(1);
end Main;
