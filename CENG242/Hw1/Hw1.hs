module Hw1 (next) where -- DO NOT CHANGE THIS

next :: [[String]] -> [[String]]
-- IMPLEMENT NEXT FUNCTION HERE --


next x = helper x x

helper :: [[String]] -> [[String]] -> [[String]]
helper x (a:as) = if (length as == 0) then [nexttwo x a (length as, (length a - 1))] else helper x as ++ [nexttwo x a (length as, (length a - 1))]

nexttwo :: [[String]] -> [String] -> (Int, Int) -> [String]
nexttwo x a (f,-1) = []
nexttwo x (a:as) (f,g) = if (length x == 1) then [x!!f!!g] else if (x!!f!!g == "B") && ((adjoint x (f,g) < 2) || (adjoint x (f,g) > 3)) then nexttwo x as (f,g-1) ++ ["W"] else if (x!!f!!g == "W") && (adjoint x (f,g) == 3) then nexttwo x as (f,g-1) ++ ["B"] else nexttwo x as (f,g-1) ++ [x!!f!!g]

 
adjoint :: [[String]] -> (Int, Int) -> Int
adjoint a (x,y) = (contyan a (x,y) + contdik a (x,y) + contucap a (x,y) + contycap a (x,y))

reptwo :: [[String]] -> [[String]]
reptwo (x:xs) = if (x == []) then [[]] else rep x : reptwo xs

rep :: [String]-> [String]
rep [] = []
rep (x:xs) = x : rep xs

gg :: [[String]] -> (Int,Int) -> String
gg a (f,g) = a!!f!!g

den :: [[String]] -> [String]
den (a:as) = a


contyan :: [[String]] -> (Int,Int) -> Int
contyan a (0,0) = if (length a > 1) && (a!!0!!1 == "B" || a!!0!!1 == "N") then 1 else 0
contyan a (f,0) = if (a!!f!!1 == "B" || a!!f!!1 == "N") then 1 else 0
contyan a (f,g) = if (g+1 < length a && (a!!f!!(g+1) == "B" || a!!f!!(g+1) == "N") && (a!!f!!(g-1) == "W" || a!!f!!(g-1) == "Y")) then 1 else if (g+1 < length a && (a!!f!!(g+1) == "W" || a!!f!!(g+1) == "Y") && (a!!f!!(g-1) == "B" || a!!f!!(g-1) == "N")) then 1 else if (g+1 < length a && (a!!f!!(g+1) == "B" || a!!f!!(g+1) == "N") && (a!!f!!(g-1) == "B" || a!!f!!(g-1) == "N")) then 2 else if (g+1 >= length a && (a!!f!!(g-1) == "B" || a!!f!!(g-1) == "N")) then 1 else 0


contdik :: [[String]] -> (Int,Int) -> Int
contdik a (0,0) = if (a!!1!!0 == "B" || a!!1!!0 == "N") then 1 else 0
contdik a (0,g) = if (a!!1!!g == "B" || a!!1!!g == "N") then 1 else 0
contdik a (f,g) = if (f+1 < length a && (a!!(f+1)!!g == "B" || a!!(f+1)!!g == "N") && (a!!(f-1)!!g == "W" || a!!(f-1)!!g == "Y")) then 1 else if (f+1 < length a && (a!!(f+1)!!g == "W" || a!!(f+1)!!g == "Y") && (a!!(f-1)!!g == "B" || a!!(f-1)!!g == "N")) then 1 else if (f+1 < length a && (a!!(f+1)!!g == "B" || a!!(f+1)!!g == "N") && (a!!(f-1)!!g == "B" || a!!(f-1)!!g == "N")) then 2 else if (f+1 >= length a && (a!!(f-1)!!g == "B" || a!!(f-1)!!g == "N")) then 1 else 0
  

contucap :: [[String]] -> (Int,Int) -> Int
contucap a (0,0) = 0
contucap a (f,0) = if (a!!(f-1)!!1 == "B" || a!!(f-1)!!1 == "N") then 1 else 0
contucap a (0,g) = 0
contucap a (f,g) = if (g+1 < length a && (a!!(f-1)!!(g+1) == "B" || a!!(f-1)!!(g+1) == "N") && (a!!(f-1)!!(g-1) == "W" || a!!(f-1)!!(g-1) == "Y")) then 1 else if (g+1 < length a && (a!!(f-1)!!(g+1) == "W" || a!!(f-1)!!(g+1) == "Y") && (a!!(f-1)!!(g-1) == "B" || a!!(f-1)!!(g-1) == "N")) then 1 else if (g+1 < length a && (a!!(f-1)!!(g+1) == "B" || a!!(f-1)!!(g+1) == "N") && (a!!(f-1)!!(g-1) == "B" || a!!(f-1)!!(g-1) == "N")) then 2 else if (g+1 >= length a && (a!!(f-1)!!(g-1) == "B" || a!!(f-1)!!(g-1) == "N")) then 1 else 0


contycap :: [[String]] -> (Int,Int) -> Int
contycap a (0,0) = if (a!!1!!1 == "B" || a!!1!!1 == "N") then 1 else 0
contycap a (f,0) = if (f+1 < length a && (a!!(f+1)!!1 == "B" || a!!(f+1)!!1 == "N")) then 1 else 0
contycap a (f,g) = if (g+1 < length a && f+1 < length a && (a!!(f+1)!!(g+1) == "B" || a!!(f+1)!!(g+1) == "N") && (a!!(f+1)!!(g-1) == "W" || a!!(f+1)!!(g-1) == "Y")) then 1 else if (g+1 < length a && f+1 < length a && (a!!(f+1)!!(g+1) == "W" || a!!(f+1)!!(g+1) == "Y") && (a!!(f+1)!!(g-1) == "B" || a!!(f+1)!!(g-1) == "N")) then 1 else if (g+1 < length a && f+1 < length a && (a!!(f+1)!!(g+1) == "B" || a!!(f+1)!!(g+1) == "N") && (a!!(f+1)!!(g-1) == "B" || a!!(f+1)!!(g-1) == "N")) then 2 else if (g+1 >= length a && f+1 < length a && (a!!(f+1)!!(g-1) == "B" || a!!(f+1)!!(g-1) == "N")) then 1 else if (f+1 >= length a) then 0 else 0

-- DO NOT CHANGE CODE BELOW THIS LINE! --
createFile fileName = writeFile fileName ""

writeSteps filename grid n = do
                    createFile filename
                    writeStepsHelper filename grid n n
    where writeStepsHelper filename grid n all = if n == 0 then putStrLn ((show all) ++ " steps are succesfully written to \'" ++ filename ++ "\'") else do
                                    appendFile filename ((show (next grid)) ++ "\n")
                                    writeStepsHelper filename (next grid) (n-1) all