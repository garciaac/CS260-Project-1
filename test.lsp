(defun play (decision) 
  (let* ((myptsn (points decision lastopdec)) (opptsn (points lastopdec decision)) (out (decide-aanz 1 history decision myptsn opptsn)))
  (progn
    (setf mypts (+ 100 mypts myptsn))
    (setf oppts (+ 100 oppts opptsn))
    (setf history (cdr out))
    (setf display (list lastopdec 'my-gain (+ 100 myptsn) 'my-total mypts 'op-gain (+ 100 opptsn) 'op-total oppts))
    (setf lastopdec (car out))
    display)))


(defun points (myd opd) 
  (if (or (not myd) (not opd)) 0
    (cond ((and (eq myd 'cooperate) (eq opd 'cooperate)) (- 0 (random 20)))
          ((and (eq myd 'cooperate) (eq opd 'defect)) (- 0 (random 100)))
          ((and (eq myd 'defect) (eq opd 'cooperate)) 0)
          ((and (eq myd 'defect) (eq opd 'defect)) (- 0 (+ 10 (random 50)))))))


(setf lastopdec nil)
(setf history nil)
(setf mypts -100)
(setf oppts -100)

(play nil)
