module WordTree(WordTree(Word, Subword, Root), emptyTree, getAllPaths, addWords, deleteWords, getWordsBeginWith) where

data WordTree = Word String | Subword String [WordTree] | Root [WordTree]

emptyTree :: WordTree
emptyTree = Root []
-- DO NOT MODIFY ABOVE

getAllPaths :: WordTree -> [[String]]
addWords :: WordTree -> [String] -> WordTree
deleteWords :: WordTree -> [String] -> WordTree
getWordsBeginWith :: WordTree -> String -> [String]

instance Show WordTree where
    show (Root []) = ""
    --show (Word string) = string
    show (Root ls) = (helperIns ls "" 0)

helperIns :: [WordTree] -> String -> Int -> String
helperIns [] kk nu = ""
helperIns mm@(Subword string x:xs) kk nu = (take ((nu)*2) (cycle " ")) ++ helperShow kk string ++ ":" ++ "\n" ++ helperIns x (kk ++ string) (nu+1) ++ helperIns xs kk nu
helperIns ((Word x):xs) kk nu = (take ((nu)*2) (cycle " ")) ++ helperShow kk x ++ "\n" ++ helperIns xs kk nu

helperShow :: String -> String -> String
helperShow string kk = string ++ kk

getAllPaths (Root ls) = helperGet ls [] 

helperGet :: [WordTree] -> [String] -> [[String]]
helperGet [] _ = []
helperGet (Subword string x:xs) kk = helperGet2 x (kk ++ [string]) ++ helperGet2 xs (kk)
helperGet ((Word x):xs) kk = [[x]] ++ helperGet xs kk

helperGet2 :: [WordTree] -> [String] -> [[String]]
helperGet2 [] _ = []
helperGet2 (Subword string x:xs) kk = helperGet2 x (kk ++ [string]) ++ helperGet2 xs kk
helperGet2 ((Word x):xs) kk = [kk ++ [x]] ++ helperGet2 xs kk

addWords (Root ls) [] = Root ls
addWords (Root ls) string = Root (recList ls string) 

recList :: [WordTree] -> [String] -> [WordTree]
recList ls [] = ls
recList ls (x:xs) = recList (helperAdd ls x) xs

helperAdd :: [WordTree] -> String -> [WordTree]
helperAdd [] kk = ((Word kk):[]) 
helperAdd (Subword string x:xs) kk = if (ortakElem string kk == "") && (string < kk) then (Subword string x):(helperAdd xs kk) else if (ortakElem string kk == "") && (string > kk) then (Word kk):(Subword string x:xs) else if (ortakElem string kk == "") && (string == kk) then (Subword string x:xs) else if (ortakElem string kk /= "") && ((ortakElem string kk) == string) && (length kk > length string) then (Subword (ortakElem string kk) (helperAdd x (farkElem kk (ortakElem kk string)))):xs else if (ortakElem string kk /= "") && ((ortakElem string kk) == string) && (length string > length kk) then (Subword (ortakElem kk string) (helperAdd x "")):xs else if (ortakElem string kk /= "") && ((ortakElem string kk) == string) && (length string == length kk) then (Subword (ortakElem string kk) (helperAdd x (farkElem kk (ortakElem kk string)))):xs else if (ortakElem string kk /= "") && (string < kk) then (Subword (ortakElem kk string) ((Subword (farkElem string (ortakElem kk string)) x):[(Word (farkElem kk (ortakElem string kk)))])):xs else if (ortakElem string kk /= "") && (string > kk) then (Subword (ortakElem kk string) ((Word (farkElem kk (ortakElem kk string))):[(Subword (farkElem string (ortakElem kk string)) x)])):xs else if (ortakElem string kk /= "") && (string == kk) then (Subword (ortakElem kk string) (helperAdd x "")):xs else (Subword string x:xs)
helperAdd ((Word x):xs) kk = if (ortakElem x kk == "") && (x < kk) then ((Word x):(helperAdd xs kk)) else if (ortakElem x kk == "") && (x > kk) then ((Word kk):(Word x):xs) else if (ortakElem x kk == "") && (x == kk) then ((Word x):xs) else if (ortakElem x kk == "") && (x == kk) then ((Word x):xs) else if (ortakElem x kk /= "") && (x < kk) then ((Subword (ortakElem x kk) (Word (farkElem x (ortakElem x kk)):(Word (farkElem kk (ortakElem x kk))):[])):xs) else if (ortakElem x kk /= "") && (x > kk) then ((Subword (ortakElem x kk) ((Word (farkElem kk (ortakElem x kk))):(Word (farkElem x (ortakElem x kk))):[])):xs) else if (ortakElem x kk /= "") && (x == kk) then ((Word x):xs) else ((Word x):xs)

ortakElem :: String -> String -> String
ortakElem [] _ = []
ortakElem _ [] = []
ortakElem (x:xs) (y:ys) = if (x == y) then (x : ortakElem xs ys) else ""

farkElem :: String -> String -> String
farkElem [] f = []
farkElem f [] = f
farkElem (x:xs) (y:ys) = if (x == y) then (farkElem xs ys) else ([x] ++ xs)

deleteWords (Root kk) [] = (Root kk)
deleteWords (Root kk) string = Root (helperRec kk string)

helperRec :: [WordTree] -> [String] -> [WordTree]
helperRec kk [] = kk
helperRec kk (x:xs) = helperRec (helperDel kk x "") xs

helperDel :: [WordTree] -> String -> String -> [WordTree]
helperDel [] _ _ = []
helperDel mm@((Word x):xs) string sb = if (string == x) then xs else (Word x):(helperDel xs string sb)
helperDel (Subword kk x:xs) string sb = if (ortakElem kk string) /= "" then (helperCon ((helperDel x (farkElem string (ortakElem kk string)) kk)) kk):xs  else (Subword kk x):(helperDel xs string sb)

helperCon :: [WordTree] -> String -> WordTree
helperCon x string = if (length x == 1) then (helperCon2 x string) else (Subword string x)

helperCon2 :: [WordTree] -> String -> WordTree
helperCon2 ((Subword kk x):xs) string = (Subword (string ++ kk) x)
helperCon2 ((Word kk):xs) string = (Word (string ++ kk))

isElem :: [WordTree] -> String -> String -> [WordTree]
isElem ((Subword b [Word kk, Word cc]):xs) string sb = if (string == cc) then (Word (sb ++ b ++ kk)):xs else if (string == kk) then (Word (sb ++ b ++ cc)):xs else (Subword b [Word kk, Word cc]):(helperDel xs string b) 
isElem ((Subword b [Word kk, Subword cc xx]):xs) string sb = if (string == kk) then (Subword (sb ++ b ++ cc) xx):xs else if (ortakElem string cc) == cc then (Subword b [Word kk, (helperSil (Subword cc xx) (farkElem string cc) sb)]):xs else helperDel xs string b
isElem ((Subword b [Subword cc xx, Word kk]):xs) string sb = if (string == kk) then (Subword (sb ++ b ++ cc) xx):xs else if (ortakElem string cc) == cc then (Subword b [(helperSil (Subword cc xx) (farkElem string cc) sb), Word kk]):xs else helperDel xs string b
isElem ((Subword b [Subword cc xx, Subword kk yy]):xs) string sb = if ((ortakElem string cc) == cc) then (Subword b [(helperSil (Subword cc xx) (farkElem string cc) sb), Subword kk yy]):xs else if ((ortakElem string kk) == kk) then (Subword b [Subword cc xx, (helperSil (Subword kk yy) (farkElem string kk) sb)]):xs else helperDel xs string b

helperSil :: WordTree -> String -> String -> WordTree
helperSil (Subword b [Word kk, Word cc]) string sb = if (string == cc) then (Word (sb ++ b ++ kk)) else if (string == kk) then (Word (sb ++ b ++ cc))  else (Subword b [Word kk, Word cc])
helperSil (Subword b [Word kk, Subword cc xx]) string sb = if (string == kk) then (Subword (sb ++ b ++ cc) xx) else if (ortakElem string cc) == cc then (Subword b [Word kk, (helperSil (Subword cc xx) (farkElem string cc) b)]) else (Subword b [Word kk, Subword cc xx])
helperSil (Subword b [Subword cc xx, Word kk]) string sb = if (string == kk) then (Subword (sb ++ b ++ cc) xx) else if (ortakElem string cc) == cc then (Subword b [(helperSil (Subword cc xx) (farkElem string cc) b), Word kk]) else (Subword b [Subword cc xx, Word kk])

getWordsBeginWith (Root list) string = (helperBeg list string "")

helperBeg :: [WordTree] -> String -> String -> [String]
helperBeg [] _ _ = []
helperBeg ((Word string):xs) kk ek = if (helperElem string kk) == True then [(helperEk ek string)] ++ (helperBeg xs kk ek) else helperBeg xs kk ek
helperBeg (Subword string x:xs) kk ek = if ((helperElem string kk) == True) || ((helperElement string kk) == True) then (helperBeg x (farkElem kk (ortakElem kk string)) (helperEk ek string)) ++ (helperBeg xs kk ek) else helperBeg xs kk ek

helperElem :: String -> String -> Bool
helperElem h t = if (ortakElem h t) == t then True else False

helperElement :: String -> String -> Bool
helperElement h t = if (ortakElem h t) == h then True else False

helperEk :: String -> String -> String
helperEk f t = f ++ t


<div class="friend_button"><i class="fa fa-user-plus" onClick="addFriend(' + "'" + c._id + "'" + ')"></i></div></div>')